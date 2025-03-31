#include "octree.h"
#include <algorithm>
#include <assert.h>
#include <functional>
namespace GRelated {
    namespace OCtree {

        DLooseOctree::DLooseOctree(GRUINT threshold, GRUINT maxDepth)
            : m_threshold(threshold), m_maxDepth(maxDepth),
            m_nodeCreator(std::make_unique<OctreeNodeFactory>()),
            m_root(m_nodeCreator->create(Bounding_box(), nullptr, this))
        {

        }

        DLooseOctree::~DLooseOctree()
        {
            m_root.reset(nullptr);
        }


        void DLooseOctree::queryRange(const Bounding_box& target,
            OctreeNode::ItemResult& res) const
        {
            QueryFunc func = [&target](const Bounding_box& box) -> bool
                {
                    return target.contains(box) || target.intersect(box);
                };
            m_root->query(func, res);
        }

        void DLooseOctree::rayIntersect(const Ray& ray, OctreeNode::ItemResult& res) const
        {
            QueryFunc func = [&ray](const Bounding_box& box) -> bool
                {
                    GRfloat t_min = GRfloat_min;  // 初始化进入时间为负无穷
                    GRfloat t_max = GRfloat_max;   // 初始化退出时间为正无穷

                    for (int axis = 0; axis < 3; axis++) {  // 分别处理x、y、z轴
                        float inv_d = 1.0f / ray.dir[axis];  // 方向分量的倒数
                        if (GRfloat_equal(inv_d, 0.0f)) { // 方向分量为0（需处理浮点误差）
                            if (ray.pos[axis] < box.min_pt()[axis] || ray.pos[axis] > box.max_pt()[axis]) {
                                return false; // 不相交
                            }
                            continue; // 跳过当前轴的计算
                        }
                        float t0 = (box.min_pt()[axis] - ray.pos[axis]) * inv_d;  // 进入时间
                        float t1 = (box.max_pt()[axis] - ray.pos[axis]) * inv_d;  // 退出时间

                        // 如果方向分量为负，交换t0和t1
                        if (inv_d < 0.0f)
                        {
                            std::swap(t0, t1);
                        }

                        // 更新全局的t_min和t_max
                        t_min = std::max(t_min, t0);  // 进入时间取各轴的最大值
                        t_max = std::min(t_max, t1);  // 退出时间取各轴的最小值

                        // 如果当前轴的退出时间 < 进入时间，说明不相交
                        if (t_max <= t_min)
                        {
                            return false;
                        }
                    }

                    // 最终检查退出时间是否非负
                    return t_max > 0.0f || GRfloat_equal(t_max, 0.0f);
                };
            m_root->query(func, res);
        }

        void DLooseOctree::update()
        {
            m_updateTimes++;
            if (!m_root->boundingBox().valid())
            {
                Bounding_box initBbox;
                for (auto& pItem : m_addList)
                {
                    initBbox.expand(pItem->getBbox().x_min, pItem->getBbox().y_min,
                        pItem->getBbox().z_min);
                    initBbox.expand(pItem->getBbox().x_max, pItem->getBbox().y_max,
                        pItem->getBbox().z_max);
                }
                m_root->setBoundingBox(initBbox);
            }
            for (auto& itemPtr : m_addList)
            {
                _add(itemPtr);
            }

            for (auto& ItemPtr : m_removedList)
            {
                _remove(ItemPtr);
            }

            for (auto& ItemPtr : m_modfiyList)
            {
                _modify(ItemPtr);
            }

            m_addList.swap(decltype(m_addList) {});
            m_removedList.swap(decltype(m_removedList) {});
            m_modfiyList.swap(decltype(m_modfiyList) {});
            if (m_updateTimes % 5 == 0)
            {
                _dealNodes();
            }
        }

        void DLooseOctree::cacheSubdivide(OctreeNode* pNode)
        {
            m_subdivided.insert(pNode);
        }

        void DLooseOctree::cacheMerge(OctreeNode* pNode)
        {
            m_merged.insert(pNode);
        }

        void DLooseOctree::ensureOutCache(OctreeNode* pNode)
        {
            m_subdivided.erase(pNode);
            m_merged.erase(pNode);
        }

        bool DLooseOctree::add(ItemPtr item)
        {
            if (item->associatedNode() != nullptr)
            {
                return false;
            }
            m_addList.push_back(item);
            return true;
        }

        bool DLooseOctree::remove(ItemPtr item)
        {
            if (item->associatedNode() == nullptr)
            {
                return false;
            }
            m_removedList.push_back(item);
            return true;
        }

        bool DLooseOctree::modify(ItemPtr item)
        {
            if (item->associatedNode() == nullptr)
            {
                return false;
            }
            m_modfiyList.push_back(item);
            return true;
        }

        void DLooseOctree::_add(ItemPtr pItem)
        {
            if (pItem->associatedNode() != nullptr)
            {
                return;
            }
            if (!m_root->looseBoundingBox().contains(pItem->getBbox()))
            {
                _expand(pItem);
            }
            std::vector<OctreeNode*> nodeStack;
            nodeStack.push_back(m_root.get());
            while (!nodeStack.empty())
            {
                auto curNode = nodeStack.back();
                nodeStack.pop_back();
                if (!curNode->looseBoundingBox().contains(pItem->getBbox()))
                {
                    continue;
                }
                if (curNode->isLeaf())
                {
                    curNode->insert(pItem);
                    break;
                }
                for (int i = 0; i < 8; ++i)
                {
                    nodeStack.push_back(curNode->getChild(i));
                }
            }
            assert(pItem->associatedNode() != nullptr);

        }

        void DLooseOctree::_expand(ItemPtr pItem)
        {
            auto curRootBbox = m_root->boundingBox();

            auto itemBbox = pItem->getBbox();
            // 确定扩大一倍包围盒时的方向
            unsigned char dir(0);
            unsigned char index(0);
            if (itemBbox.x_min > curRootBbox.x_min || itemBbox.x_max > itemBbox.x_max)
            {
                dir |= 0x1;
                index &= ~0x1;
            }
            else
            {
                dir &= ~0x1;
                index |= 0x1;
            }

            if (itemBbox.y_min > curRootBbox.y_min || itemBbox.y_max > itemBbox.y_max)
            {
                dir |= 0x2;
                index &= ~0x2;
            }
            else
            {
                dir &= ~0x2;
                index |= 0x2;
            }

            if (itemBbox.z_min > curRootBbox.z_min || itemBbox.z_max > itemBbox.z_max)
            {
                dir |= 0x4;
                index &= ~0x4;
            }
            else
            {
                dir &= ~0x4;
                index |= 0x4;
            }

            while (!m_root->boundingBox().contains(itemBbox))
            {
                Bounding_box newBbox;
                auto oldBbox = m_root->boundingBox();
                auto x_len = oldBbox.x_max - oldBbox.x_min;
                auto y_len = oldBbox.y_max - oldBbox.y_min;
                auto z_len = oldBbox.z_max - oldBbox.z_min;
                if (dir & 0x1)
                {
                    newBbox.x_min = oldBbox.x_min;
                    newBbox.x_max = oldBbox.x_min + x_len * 2.f;
                }
                else
                {
                    newBbox.x_max = oldBbox.x_max;
                    newBbox.x_min = oldBbox.x_max - x_len * 2.f;
                }
                if (dir & 0x2)
                {
                    newBbox.y_min = oldBbox.y_min;
                    newBbox.y_max = oldBbox.y_min + y_len * 2.f;
                }
                else
                {
                    newBbox.y_max = oldBbox.y_max;
                    newBbox.y_min = oldBbox.y_max - y_len * 2.f;
                }
                if (dir & 0x4)
                {
                    newBbox.z_min = oldBbox.z_min;
                    newBbox.z_max = oldBbox.z_min + z_len * 2.f;
                }
                else
                {
                    newBbox.z_max = oldBbox.z_max;
                    newBbox.z_min = oldBbox.z_max - z_len * 2.f;
                }
                auto newRoot = m_nodeCreator->create(newBbox, nullptr, this);
                newRoot->subdivide();
                m_root->setParent(newRoot.get());
                m_root->getChildren()[index].swap(m_root);
                m_root = std::move(newRoot);
            }
        }

        void DLooseOctree::_remove(ItemPtr pItem)
        {
            if (pItem->associatedNode() == nullptr)
            {
                return;
            }
            auto node = pItem->associatedNode();
            assert(node->erase(pItem));
        }

        void DLooseOctree::_modify(ItemPtr pItem)
        {
            if (pItem->associatedNode() == nullptr)
            {
                return;
            }
            auto curNode = pItem->associatedNode();
            if (curNode->looseBoundingBox().contains(pItem->getBbox()))
            {
                curNode->erase(pItem);
                curNode->insert(pItem);
            }
            else
            {
                curNode->erase(pItem);
                _add(pItem);
            }
        }

        void DLooseOctree::_dealNodes()
        {
            for (auto curNode : m_subdivided)
            {
                if (curNode->checkSubdivide())
                {
                    curNode->subdivide();
                }
            }
            m_subdivided.clear();
            for (auto curNode : m_merged)
            {
                if (curNode->checkMerge())
                {
                    curNode->merge();
                }
            }
            m_merged.clear();
        }

    }
}