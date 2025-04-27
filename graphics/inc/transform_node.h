/*场景图中的连接性质节点transformNode*/
#ifndef _TRANSFORM_NODE_H
#define _TRANSFORM_NODE_H

#include "node.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace GRelated {

    class TransformNode : public Node<TransformNode>
    {
    public:
        explicit TransformNode(const glm::mat4& mat)
            : m_mat(mat)
        {

        }
        ~TransformNode() = default;

        NodeType nodeType() const override
        {
            return NodeBase::kTransform;
        }

        // 节点的连接
        void addParent(NodeBase*) override;
        void removeParent(NodeBase*) override;
        NodeBase* getParent(GRUINT32 idx) const override;
        void addChild(NodePtr) override;
        void removeChild(NodePtr) override;
        NodePtr getChild(GRUINT32 idx) const override;
        GRUINT32 childNum() const override
        {
            return 1;
        }

        // 节点访问
        void accept(NodeVisitor&) override;
        // 访问父亲节点
        void ascend(NodeVisitor&) override;
        // 访问孩子
        void traverse(NodeVisitor&) override;

        void setMat(const glm::mat4& mat)
        {
            m_mat = mat;
        }

        glm::mat4 mat() const
        {
            return m_mat;
        }

    private:
        glm::mat4 m_mat;
        NodeBase* m_parent;
        NodePtr m_child;
    };
}
#endif // !_TRANSFORM_NODE_H
