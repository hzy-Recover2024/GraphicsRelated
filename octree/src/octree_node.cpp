#include "octree_node.h"
namespace GRelated {
    namespace OCtree {
        OctreeNodePtr OctreeNodeFactory::create(const Bounding_box& bbox,
            OctreeNode* parent, Octree* owner) const
        {
            OctreeNode* newPtr = OctreeNodeAllocator::getinstance().allocate();
            new(newPtr) OctreeNode(bbox, parent, owner);
            OctreeNodeRelease deleter;
            return OctreeNodePtr(newPtr, deleter);
        }
    }
}

