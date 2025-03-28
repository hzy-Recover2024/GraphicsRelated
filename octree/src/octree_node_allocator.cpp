#include "octree_node_allocator.h"
#include "octree_node.h"
namespace GRelated {
    namespace OCtree {
        OctreeNodeAllocator& OctreeNodeAllocator::getinstance()
        {
            static OctreeNodeAllocator instance;
            return instance;
        }

        OctreeNode* OctreeNodeAllocator::allocate()
        {
            return m_memoryPool.allocate();
        }

        void OctreeNodeAllocator::free(OctreeNode* ptr)
        {
            m_memoryPool.deallocate(ptr);
        }

        void OctreeNodeRelease::operator()(OctreeNode* ptr) const
        {
            ptr->~OctreeNode();
            OctreeNodeAllocator::getinstance().free(ptr);
        }

    }

}