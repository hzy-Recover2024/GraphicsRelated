/*动态松散八叉树定义*/
#ifndef _OCTREE_H
#define _OCTREE_H
#include "GR_math.h"
#include "octree_item.h"
#include "octree_node.h"
#include <deque>
namespace GRelated {
    namespace OCt {
        //动态松散八叉树
        class DLooseOctree {
        public:
            DLooseOctree(GRUINT, GRUINT);
        private:
            GRUINT m_maxDepth;
            GRUINT m_threshold;
            OctreeNode* m_root = nullptr;
            // 待插入列表
            std::vector<ItemRef> m_addList;
            // 待删除列表
            std::vector<ItemRef> m_removedList;
            // 待更新列表
            std::vector<ItemRef> m_modfiyList;
            // 待分割
            std::vector<OctreeNode*> m_subdivided;
            // 待合并
            std::vector<OctreeNode*> m_merged;

            //TODO :存储一个node的分配器
        };
    }
}
#endif // !_OCTREE_H
