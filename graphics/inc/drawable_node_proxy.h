/*场景图中的drawableNode经过路径解析后生成的场景树drawableNodeProxy*/
#ifndef _DRAWABLE_NODE_PROXY_H
#define _DRAWABLE_NODE_PROXY_H
#include "b_box.h"
#include "drawable_node.h"
namespace GRelated {
    class DrawableNode;

    class DrawableNodeProxy : public std::enable_shared_from_this<DrawableNodeProxy> {
    public:
        DrawableNodeProxy(DrawableNode* pNode, const MatrixDummy& mat);
        ~DrawableNodeProxy();

        Bounding_box boundingBox() const;
        const MatrixDummy& mat() const
        {
            return m_mat;
        }

        const StateSetDummy& attribute() const
        {
            return m_attribute;
        }

        void addChild(NodeProxyPtr child)
        {
            m_childList.push_back(child);
        }
        void removeChild(NodeProxyPtr child)
        {
            m_childList.erase(std::remove(
                m_childList.begin(), m_childList.end(), child),
                m_childList.end());
        }
        void addParent(DrawableNodeProxy* par)
        {
            m_parent = par;
        }
        void removeParent(DrawableNodeProxy* par)
        {
            m_parent = nullptr;
        }

    private:
        DrawableNode* m_associatedNode = nullptr;
        MatrixDummy m_mat;
        StateSetDummy m_attribute;
        DrawableNodeProxy* m_parent;
        std::vector<NodeProxyPtr> m_childList;
    };
}
#endif // !_DRAWABLE_NODE_PROXY_H
