#include "transform_node.h"
#include "node_visitor.h"
#include <assert.h>
namespace GRelated {


    void TransformNode::addParent(NodeBase* pNode)
    {
        m_parent = pNode;
    }

    void TransformNode::removeParent(NodeBase* pNode)
    {
        assert(pNode == m_parent);
        m_parent = nullptr;
    }

    GRelated::NodeBase* TransformNode::getParent(GRUINT32) const
    {
        return m_parent;
    }

    void TransformNode::addChild(NodePtr pNode)
    {
        m_child = pNode;
    }

    void TransformNode::removeChild(NodePtr pNode)
    {
        assert(pNode == m_child);
        m_child.reset();
    }

    GRelated::NodePtr TransformNode::getChild(GRUINT32 idx) const
    {
        return m_child;
    }

    void TransformNode::accept(NodeVisitor& nv)
    {
        nv.apply(*this);
    }

    void TransformNode::ascend(NodeVisitor& nv)
    {
        if (m_parent != nullptr)
        {
            m_parent->accept(nv);
        }
    }

    void TransformNode::traverse(NodeVisitor& nv)
    {
        if (m_child != nullptr)
        {
            m_child->accept(nv);
        }
    }

}