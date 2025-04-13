/*场景图中的连接性质节点transformNode*/
#ifndef _TRANSFORM_NODE_H
#define _TRANSFORM_NODE_H

#include "node.h"

namespace GRelated {

    class TransformNode : public Node<TransformNode>
    {
    public:
        explicit TransformNode(const MatrixDummy& mat)
            : m_mat(mat)
        {

        }
        ~TransformNode() = default;

        // 节点的连接
        void addParent(NodeBase*) override;
        void removeParent(NodeBase*) override;
        NodeBase* getParent(GRUINT32 idx) const override;
        void addChild(NodePtr) override;
        void removeChild(NodePtr) override;
        NodePtr getChild(GRUINT32 idx) const override;

        // 节点访问
        void accept(NodeVisitor&) override;
        // 访问父亲节点
        void ascend(NodeVisitor&) override;
        // 访问孩子
        void traverse(NodeVisitor&) override;

        void setMat(const MatrixDummy& mat)
        {
            m_mat = mat;
        }

        MatrixDummy mat() const
        {
            return m_mat;
        }

    private:
        MatrixDummy m_mat;
        NodeBase* m_parent;
        NodePtr m_child;
    };
}
#endif // !_TRANSFORM_NODE_H
