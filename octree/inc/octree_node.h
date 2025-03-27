/*松散八叉树节点定义*/
#ifndef _OCTREE_NODE_H
#define _OCTREE_NODE_H

#include <array>
#include <functional>
#include <memory>
#include <vector>
#include "b_box.h"
namespace GRelated {
    namespace OCt {
        class OctreeItem;
        class Octree;
        constexpr auto LOOSE_FACTOR = 1.2F;
        using ItemRef = std::shared_ptr<OctreeItem>;
        using QueryFunc = std::function<bool(const Bounding_box&)>;
        // TODO定义一个自带删除器的unique_ptr<Ocnode>
        class OctreeNode {
        public:
            OctreeNode(const Bounding_box&, OctreeNode*, Octree*);
            ~OctreeNode();

            // 插入节点，若插入失败返回false
            bool insert(const ItemRef& item);

            // 移除节点，若失败返回false
            bool erase(const ItemRef& item);

            // 节点分裂(分割)
            void subdivide();

            // 递归向上合并节点
            void merge();

            //核心功能：利用空间关系加速的查询
            void query(QueryFunc func, std::vector<ItemRef>& res);
        private:
            Bounding_box m_bbox;
            std::array<OctreeNode*, 8> m_children; //TODO 八个孩子存储的是自带删除器的unique_ptr
            OctreeNode* m_parent = nullptr;
            Octree* m_owner = nullptr;
            std::vector<std::weak_ptr<OctreeItem>> m_items;
        };
    }
}
#endif // !_OCTREE_NODE_H
