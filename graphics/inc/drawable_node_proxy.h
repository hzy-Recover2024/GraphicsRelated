/*场景图中的drawableNode经过路径解析后生成的场景树drawableNodeProxy*/
#ifndef _DRAWABLE_NODE_PROXY_H
#define _DRAWABLE_NODE_PROXY_H
#include "b_box.h"
#include "drawable_node.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace GRelated {
    class DrawableNode;

    class DrawableNodeProxy : public std::enable_shared_from_this<DrawableNodeProxy> {
    public:
        DrawableNodeProxy(DrawableNode* pNode, const glm::mat4& mat);
        ~DrawableNodeProxy();

        Bounding_box boundingBox() const;


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

        DrawableNodeProxy* parent() const
        {
            return m_parent;
        }

        void removeParent()
        {
            m_parent = nullptr;
        }

        const glm::mat4& mat() const
        {
            return m_mat;
        }

        const glm::vec3& color() const
        {
            return m_color;
        }

        DrawableNode* associatedNode() const
        {
            return m_associatedNode;
        }

    private:
        DrawableNode* m_associatedNode = nullptr;
        glm::mat4 m_mat;
        glm::vec3 m_color;
        StateSetDummy m_attribute;
        DrawableNodeProxy* m_parent = nullptr;
        std::vector<NodeProxyPtr> m_childList;
    };
}
#endif // !_DRAWABLE_NODE_PROXY_H
