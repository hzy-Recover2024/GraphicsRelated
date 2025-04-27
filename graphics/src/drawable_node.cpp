#include "drawable_node.h"
#include "node_visitor.h"
#include "drawable_node_proxy.h"
namespace GRelated {
   
    DrawableNode::DrawableNode(const std::vector<std::shared_ptr<GRGLMesh>>& meshes)
    {
        for (auto& mesh : meshes)
        {
            m_geoDatas.push_back(mesh);
        }
    }

    void DrawableNode::addParent(NodeBase* pNode)
    {
        auto pos = std::lower_bound(m_parentList.begin(), m_parentList.end(), pNode);
        if (pos != m_parentList.end() && *pos == pNode)
        {
            return;
        }
        m_parentList.insert(pos, pNode);
    }

    void DrawableNode::removeParent(NodeBase* pNode)
    {
        auto pos = lower_bound(m_parentList.begin(), m_parentList.end(), pNode);
        if (pos != m_parentList.end() && *pos == pNode)
        {
            m_parentList.erase(pos);
        }
    }

    GRelated::NodeBase* DrawableNode::getParent(GRUINT32 idx) const
    {
        return m_parentList.at(idx);
    }

    void DrawableNode::addChild(NodePtr pNode)
    {
        auto pos = std::lower_bound(m_childList.begin(), m_childList.end(), pNode);
        if (pos != m_childList.end() && *pos == pNode)
        {
            return;
        }
        m_childList.insert(pos, pNode);
    }

    void DrawableNode::removeChild(NodePtr pNode)
    {
        auto pos = std::lower_bound(m_childList.begin(), m_childList.end(), pNode);
        if (pos != m_childList.end() && *pos == pNode)
        {
            m_childList.erase(pos);
        }
    }

    GRelated::NodePtr DrawableNode::getChild(GRUINT32 idx) const
    {
        return m_childList.at(idx);
    }

    void DrawableNode::accept(NodeVisitor& nv)
    {
        nv.apply(*this);
    }

    void DrawableNode::ascend(NodeVisitor& nv)
    {
        for (auto& parent : m_parentList)
        {
            parent->accept(nv);
        }
    }

    void DrawableNode::traverse(NodeVisitor& nv)
    {
        for (auto& child : m_childList)
        {
            child->accept(nv);
        }
    }

    void DrawableNode::attach(NodeProxyPtr pNode)
    {
        auto pos = std::lower_bound(m_observer.begin(), m_observer.end(),
            std::weak_ptr<DrawableNodeProxy>(pNode),
            [](const std::weak_ptr<DrawableNodeProxy>& left, const std::weak_ptr<DrawableNodeProxy>& right)
            {
                return left.lock() < right.lock();
            });
        if (pos != m_observer.end() && pos->lock() == pNode)
        {
            m_observer.insert(pos, std::weak_ptr<DrawableNodeProxy>(pNode));
        }
    }

    void DrawableNode::detach(NodeProxyPtr pNode)
    {
        auto pos = std::lower_bound(m_observer.begin(), m_observer.end(),
            std::weak_ptr<DrawableNodeProxy>(pNode),
            [](const std::weak_ptr<DrawableNodeProxy>& left, const std::weak_ptr<DrawableNodeProxy>& right)
            {
                return left.lock() < right.lock();
            });
        if (pos != m_observer.end() && pos->lock() == pNode)
        {
            m_observer.erase(pos);
        }
    }

    GRUINT32 DrawableNode::vertexCount() const
    {
        if (m_vertexCount != 0)
        {
            return m_vertexCount;
        }
        for (const auto& mesh : m_geoDatas)
        {
            auto pMesh = mesh.lock();
            if (pMesh)
            {
                m_vertexCount += pMesh->vertices().size();
                m_indexCount += pMesh->indices().size();
            }
        }
        return m_vertexCount;
    }

    GRUINT32 DrawableNode::indexCount() const
    {
        if (m_indexCount != 0)
        {
            return m_indexCount;
        }
        for (const auto& mesh : m_geoDatas)
        {
            auto pMesh = mesh.lock();
            if (pMesh)
            {
                m_vertexCount += pMesh->vertices().size();
                m_indexCount += pMesh->indices().size();
            }
        }
        return m_indexCount;
    }

}

