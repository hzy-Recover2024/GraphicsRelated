#include "drawable_node_proxy.h"
#include "drawable_node.h"
#include <assert.h>
namespace GRelated {

    DrawableNodeProxy::DrawableNodeProxy(DrawableNode* pNode, const MatrixDummy& mat)
        : m_associatedNode(pNode), m_mat(mat)
    {
        m_associatedNode->attach(shared_from_this());
    }


    DrawableNodeProxy::~DrawableNodeProxy()
    {
        assert(m_associatedNode != nullptr);
        m_associatedNode->detach(shared_from_this());
    }

    GRelated::Bounding_box DrawableNodeProxy::boundingBox() const
    {
        // compute world position bounding box
        return {};
    }

}