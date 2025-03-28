/*八叉树节点的工厂和生命周期管理*/
#ifndef _OCTREE_NODE_ALLOCATOR_H
#define _OCTREE_NODE_ALLOCATOR_H

#include "memory_pool.h"
#include <memory>
namespace GRelated {
    namespace OCtree {
        class OctreeNode;
        class OctreeNodeAllocator {
        public:
            ~OctreeNodeAllocator()
            {

            }

            static OctreeNodeAllocator& getinstance();

            OctreeNode* allocate();
            void free(OctreeNode*);
        private:
            OctreeNodeAllocator() = default;
            MemoryPool<OctreeNode> m_memoryPool;
        };

        struct OctreeNodeRelease
        {
            void operator()(OctreeNode*) const;
        };

        using OctreeNodePtr = std::unique_ptr<OctreeNode, OctreeNodeRelease>;

    }
}
#endif // !_OCTREE_NODE_ALLOCATOR_H
