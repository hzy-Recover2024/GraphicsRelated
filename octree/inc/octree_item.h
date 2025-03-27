/*°Ë²æÊ÷µÄitem¶¨Òå*/
#ifndef _OCTREE_ITEM_H
#define _OCTREE_ITEM_H

#include <memory>

#include "b_box.h"
namespace GRelated {
    namespace OCt {
        class OctreeNode;
        class OctreeItem {
        public:
            OctreeItem() = default;
            virtual ~OctreeItem();

            virtual Bounding_box getBbox() const = 0;

            OctreeNode* associatedNode() const
            {
                return m_associatedNode;
            }

            void attach(OctreeNode* node)
            {
                m_associatedNode = node;
            }

            void detach()
            {
                m_associatedNode = nullptr;
            }
        private:
            OctreeNode* m_associatedNode = nullptr;
        };

        class OctreeItemFactory {
        public:
            virtual std::shared_ptr<OctreeItem> create() = 0;
            virtual ~OctreeItemFactory() = default;
        };
    }
}
#endif // !_OCTREE_ITEM_H
