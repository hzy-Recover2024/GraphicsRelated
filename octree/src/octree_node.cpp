#include "octree_node.h"
#include "octree_item.h"
#include "octree.h"
#include <assert.h>
namespace GRelated {
    namespace OCtree {
        OctreeNodePtr OctreeNodeFactory::create(const Bounding_box& bbox,
            OctreeNode* parent, DLooseOctree* owner) const
        {
            OctreeNode* newPtr = OctreeNodeAllocator::getinstance().allocate();
            new(newPtr) OctreeNode(bbox, parent, owner);
            OctreeNodeRelease deleter;
            return OctreeNodePtr(newPtr, deleter);
        }

        OctreeNode::OctreeNode(const Bounding_box& bbox, OctreeNode* parent, DLooseOctree* owner)
            : m_bbox(bbox), m_parent(parent), m_owner(owner)
        {

        }

        OctreeNode::~OctreeNode()
        {
            assert(m_items.empty());
            if (m_state.test(kToSubdivied) || m_state.test(kToMerged))
            {
                m_owner->ensureOutCache(this);
            }

        }

        bool OctreeNode::insert(ItemPtr item)
        {
            auto itemBbox = item->getBbox();
            //用松散包围盒判断
            if (!looseBoundingBox().contains(itemBbox))
            {
                return false;
            }
            m_items.emplace_back(item);
            item->attach(this);
            // 若不是叶节点或者是叶节点但item数没超过阈值
            if (!isLeaf() || m_items.size() < m_owner->threshold())
            {
                return true;
            }
            // 标记为待分割
            if (!m_state.test(kToSubdivied))
            {
                m_owner->cacheSubdivide(this);
                m_state.set(kToSubdivied, true);
            }
            return true;
        }

        bool OctreeNode::erase(ItemPtr item)
        {
            if (item->associatedNode() == nullptr)
            {
                return false;
            }
            // 删除对应的item
            item->detach();
            m_items.erase(std::remove_if(m_items.begin(), m_items.end(),
                [&item](const std::weak_ptr<OctreeItem>& val)
                {
                    return val.lock() == item;
                }), m_items.end());
            //若当前是叶节点，检查父亲是否可以合并
            if (isLeaf())
            {
                if (m_parent->checkMerge() && !m_parent->m_state.test(kToMerged))
                {
                    m_parent->m_state.set(kToMerged, true);
                    m_owner->cacheMerge(m_parent);
                }
            }
            return true;
        }

        void OctreeNode::subdivide()
        {
            m_state.reset(kToSubdivied);
            if (!isLeaf())
            {
                return;
            }
            // 创建八个子节点
            for (GRUINT32 i = 0; i < 8; ++i)
            {
                Bounding_box childBbox;
                auto x_stride = (m_bbox.x_max - m_bbox.x_min) * 0.5f;
                auto y_stride = (m_bbox.y_max - m_bbox.y_min) * 0.5f;
                auto z_stride = (m_bbox.z_max - m_bbox.z_min) * 0.5f;
                // 位于原box中心的x轴的正负
                if (i & 0x1)
                {
                    childBbox.x_min = m_bbox.x_min + x_stride;
                    childBbox.x_max = m_bbox.x_max;
                }
                else
                {
                    childBbox.x_min = m_bbox.x_min;
                    childBbox.x_max = m_bbox.x_min + x_stride;
                }
                // 位于原box中心的y轴的正负
                if (i & 0x2)
                {
                    childBbox.y_min = m_bbox.y_min + y_stride;
                    childBbox.y_max = m_bbox.y_max;
                }
                else
                {
                    childBbox.y_min = m_bbox.y_min;
                    childBbox.y_max = m_bbox.y_min + y_stride;
                }
                // 位于原box中心的z轴的正负
                if (i & 0x4)
                {
                    childBbox.z_min = m_bbox.z_min + z_stride;
                    childBbox.z_max = m_bbox.z_max;
                }
                else
                {
                    childBbox.z_min = m_bbox.z_min;
                    childBbox.z_max = m_bbox.z_min + z_stride;
                }
                assert(childBbox.valid());
                m_children[i] = m_owner->nodeCreator().create(childBbox, this, m_owner);
            }
            // 下放item
            decltype(m_items) copy;
            m_items.swap(copy);
            for (const auto& item : copy)
            {
                auto itemPtr = item.lock();
                itemPtr->detach();
                for (auto& child : m_children)
                {
                    if (child->looseBoundingBox().contains(itemPtr->getBbox()))
                    {
                        assert(child->insert(itemPtr));
                        break;
                    }
                }
                // 位于交界处的item，不下放
                if (itemPtr->associatedNode() == nullptr)
                {
                    itemPtr->attach(this);
                    m_items.emplace_back(item);
                }
            }
            return;
        }

        bool OctreeNode::checkSubdivide() const
        {
            GRUINT32 depth = 0;
            auto parent = m_parent;
            while (parent != nullptr)
            {
                parent = parent->m_parent;
                depth++;
            }
            return depth < m_owner->maxDepth() && m_items.size() > m_owner->threshold();
        }

        void OctreeNode::merge()
        {
            m_state.reset(kToMerged);
            // 收集孩子的所有item并删除孩子
            for (auto& child : m_children)
            {
                for (const auto& item : child->m_items)
                {
                    item.lock()->attach(this);
                    m_items.push_back(item);
                }
                child->m_items.clear();
                child.reset(nullptr);
            }
            // 递归向上检查父亲是否需要合并
            // 若父亲没被加入到待合并列表并且需要合并
            if (!m_parent->m_state.test(kToMerged) && m_parent->checkMerge())
            {
                m_parent->merge();
            }
            return;
        }

        bool OctreeNode::checkMerge() const
        {
            GRUINT32 itemsCount = m_items.size();
            for (const auto& child : m_children)
            {
                itemsCount += child->m_items.size();
                if (!child->isLeaf())
                {
                    return false;
                }
            }
            return itemsCount < m_owner->threshold();
        }

        void OctreeNode::query(QueryFunc func, std::vector<ItemPtr>& res) const
        {
            // 先对松散包围盒进行测试
            if (!func(looseBoundingBox()))
            {
                return;
            }

            if (func(m_bbox))
            {
                for (const auto& item : m_items)
                {
                    res.push_back(item.lock());
                }
                if (!isLeaf())
                {
                    for (const auto& child : m_children)
                    {
                        child->query(func, res);
                    }
                }

            }
            return;
        }

    }
}

