/*场景图的遍历器定义*/
#ifndef _NODE_VISITOR_H
#define _NODE_VISITOR_H

#include "node.h"
namespace GRelated {
    class NodeBase;
    class TransformNode;
    class DrawableNode;
    class DrawableNodeProxy;
    enum NodeVisitorType
    {
        kBuildSceneTree,
        kUpdate
    };
    class MatrixDummy;

    class NodeVisitor {
    public:
        explicit NodeVisitor(NodeVisitorType type)
            : m_type(type)
        {

        }
        ~NodeVisitor() = default;
        void pushNode(NodeBase*);
        void popNode();
        const std::vector<NodeBase*>& nodePath() const
        {
            return m_path;
        }
        
        void apply(DrawableNode&);
        void apply(TransformNode&);

    private:
        NodeVisitorType m_type;
        std::vector<NodeBase*> m_path;
        std::vector<DrawableNodeProxy*> m_nodeProxyPath;
        std::vector<MatrixDummy> m_matAcc;
    };
}
#endif // !_NODE_VISITOR_H

