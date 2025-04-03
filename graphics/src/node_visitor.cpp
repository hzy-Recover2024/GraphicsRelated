#include "node_visitor.h"
#include "drawable_node.h"
#include "transform_node.h"
#include "drawable_node_proxy.h"
namespace GRelated {

    void NodeVisitor::pushNode(NodeBase* pNode)
    {
        m_path.push_back(pNode);
    }

    void NodeVisitor::popNode()
    {
        m_path.pop_back();
    }

    void NodeVisitor::apply(DrawableNode& node)
    {
        pushNode(&node);
        if (m_type == kBuildSceneTree)
        {
            if (node.dirty())
            {
                // create nodeProxy
                auto newNodeProxy = NodeProxyPtr(new DrawableNodeProxy(&node, m_matAcc.back()));
                if (!m_nodeProxyPath.empty())
                {
                    m_nodeProxyPath.back()->addChild(newNodeProxy);
                }
                m_nodeProxyPath.push_back(newNodeProxy.get());
                // traverse child
                node.traverse(*this);
                m_nodeProxyPath.pop_back();
            }
            else
            {
                auto count = node.observeNum();
                for (GRUINT i = 0; i < count; ++i)
                {
                    m_nodeProxyPath.push_back(node.getObserver(i).get());
                    // traverse child
                    node.traverse(*this);
                    m_nodeProxyPath.pop_back();
                }
            }
        }
        else if (m_type == kUpdate)
        {
            // notify nodeProxy update

            // traverse child
            node.traverse(*this);
        }
        popNode();
    }

    void NodeVisitor::apply(TransformNode& node)
    {
        pushNode(&node);
        if (m_type == kBuildSceneTree)
        {
            // get mat
            //m_matAcc.push_back(m_matAcc.back() * node.mat());
            // traverse child
            node.traverse(*this);
            //m_matAcc.pop_back();
        }
        else if (m_type == kUpdate)
        {
            // notify nodeProxy update

            // traverse child
            node.traverse(*this);
        }
        popNode();
    }

}

