/*松散八叉树节点定义*/
#ifndef _OCTREE_NODE_H
#define _OCTREE_NODE_H

#include <array>
#include <functional>
#include <memory>
#include <vector>
#include <bitset>
#include "b_box.h"
#include "octree_node_allocator.h"

namespace GRelated {
    namespace OCtree {
        class OctreeItem;
        class DLooseOctree;
        constexpr auto LOOSE_FACTOR = 1.2F;
        using ItemPtr = std::shared_ptr<OctreeItem>;
        using QueryFunc = std::function<bool(const Bounding_box&)>;

        class OctreeNode {
        public:
            OctreeNode(const Bounding_box& bbox, OctreeNode* parent, DLooseOctree* owner);
            ~OctreeNode();

            // 插入节点，若插入失败返回false
            bool insert(ItemPtr item);

            // 移除节点，若失败返回false
            bool erase(ItemPtr item);

            // 节点分裂(分割)
            void subdivide();

            bool checkSubdivide() const;

            // 递归向上合并节点
            void merge();

            bool checkMerge() const;

            using ItemResult = std::vector<ItemPtr>;
            //核心功能：利用空间关系加速的查询
            void query(QueryFunc func, ItemResult& res) const;

            bool isLeaf() const
            {
                return m_children[0] == nullptr;
            }

            Bounding_box boundingBox() const
            {
                return m_bbox;
            }

            Bounding_box looseBoundingBox() const
            {
                auto x_half = (m_bbox.x_max - m_bbox.x_min) * 0.5f;
                auto y_half = (m_bbox.y_max - m_bbox.y_min) * 0.5f;
                auto z_half = (m_bbox.z_max - m_bbox.z_min) * 0.5f;

                return Bounding_box(m_bbox.x_min + x_half - x_half * LOOSE_FACTOR,
                    m_bbox.y_min + y_half - y_half * LOOSE_FACTOR,
                    m_bbox.z_min + z_half - z_half * LOOSE_FACTOR,
                    m_bbox.x_min + x_half + x_half * LOOSE_FACTOR,
                    m_bbox.y_min + y_half + y_half * LOOSE_FACTOR,
                    m_bbox.z_min + z_half + z_half * LOOSE_FACTOR);
            }

            void setBoundingBox(const Bounding_box& bbox)
            {
                m_bbox = bbox;
            }

            void setParent(OctreeNode* pNode)
            {
                m_parent = pNode;
            }

            OctreeNode* getChild(GRUINT32 index)
            {
                return m_children[index].get();
            }

            std::array<OctreeNodePtr, 8>& getChildren()
            {
                return m_children;
            }

        private:
            enum NodeState : char
            {
                kToSubdivied = 0x1,  //node是否已经被标记为待分割
                kToMerged = 0x1 << 1  // node是否被标记为待合并
            };

            Bounding_box m_bbox;
            std::array<OctreeNodePtr, 8> m_children;
            OctreeNode* m_parent = nullptr;
            DLooseOctree* m_owner = nullptr;
            std::vector<std::weak_ptr<OctreeItem>> m_items;
            std::bitset<8> m_state;
        };

        class OctreeNodeFactory {
        public:
            virtual OctreeNodePtr create(const Bounding_box& bbox,
                OctreeNode* parent, DLooseOctree* owner) const;
            virtual ~OctreeNodeFactory() = default;
        };
    }
}
#endif // !_OCTREE_NODE_H
