/*动态松散八叉树定义*/
#ifndef _OCTREE_H
#define _OCTREE_H
#include "GR_math.h"
#include "octree_item.h"
#include "octree_node.h"
#include "octree_node_allocator.h"
#include <deque>
namespace GRelated {
    namespace OCtree {
        //动态松散八叉树
        class DLooseOctree {
        public:
            DLooseOctree(GRUINT, GRUINT);


            void cacheSubdivide(OctreeNode*);
            void cacheMerge(OctreeNode*);

            void ensureOutCache(OctreeNode*);

            const OctreeNodeFactory& nodeCreator() const
            {
                return *m_nodeCreator;
            }

            OctreeNodeFactory& nodeCreator()
            {
                return *m_nodeCreator;
            }

            GRUINT threshold() const
            {
                return m_threshold;
            }

            GRUINT maxDepth() const
            {
                return m_maxDepth;
            }
        private:
            GRUINT m_maxDepth;
            GRUINT m_threshold;
            std::unique_ptr<OctreeNodeFactory> m_nodeCreator;
            // 待插入列表
            std::vector<ItemPtr> m_addList;
            // 待删除列表
            std::vector<ItemPtr> m_removedList;
            // 待更新列表
            std::vector<ItemPtr> m_modfiyList;
            // 待分割
            std::vector<OctreeNode*> m_subdivided;
            // 待合并
            std::vector<OctreeNode*> m_merged;
            OctreeNode* m_root = nullptr;
        };
    }
}
#endif // !_OCTREE_H
