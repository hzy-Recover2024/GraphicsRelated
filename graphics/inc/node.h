/*场景图节点Node的基类定义*/
#ifndef _NODE_H
#define _NODE_H

#include "GR_math.h"
#include "memory_pool.h"
#include <memory>
namespace GRelated {
    class NodeVisitor;
    class NodeBase;
    using NodePtr = std::shared_ptr<NodeBase>;

    class NodeBase {
    public:
        virtual ~NodeBase() = default;
        enum NodeType : char
        {
            kTransform,
            kDrawable
        };

        virtual NodeType nodeType() const = 0;

        // 节点的连接
        virtual void addParent(NodeBase*) = 0;
        virtual void removeParent(NodeBase*) = 0;
        virtual NodeBase* getParent(GRUINT32 idx) const = 0;
        virtual void addChild(NodePtr) = 0;
        virtual void removeChild(NodePtr) = 0;
        virtual NodePtr getChild(GRUINT32 idx) const = 0;
        virtual GRUINT32 childNum() const = 0;

        // 节点访问
        virtual void accept(NodeVisitor&) = 0;
        // 访问父亲节点
        virtual void ascend(NodeVisitor&) = 0;
        // 访问孩子
        virtual void traverse(NodeVisitor&) = 0;
    };



    template <typename Derived>
    class Node : public NodeBase {
    public:
        Node() = default;
        ~Node() = default;

        static MemoryPool<Derived> s_memPool;

        void* operator new(size_t size)
        {
            if (size != sizeof(Derived))
            {
                return ::operator new(size);
            }
            return s_memPool.allocate();
        }

        void operator delete(void* ptr)
        {
            s_memPool.deallocate(static_cast<Derived*>(ptr));
        }
    };

    template <typename Derived>
    MemoryPool< Derived> Node<Derived>::s_memPool;
}
#endif // !_NODE_H

