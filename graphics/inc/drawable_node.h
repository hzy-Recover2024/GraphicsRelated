/*包含绘制对象的几何数据、属性数据的drawableNode定义*/
#ifndef _DRAWABLE_NODE_H
#define _DRAWABLE_NODE_H

#include "node.h"
#include <vector>
#include <memory>
#include "grgl_mesh.h"
namespace GRelated {
    struct GeometryDataDummy  //TODO
    {

    };
    struct StateSetDummy //TODO
    {

    };
    class DrawableNodeProxy;
    using NodeProxyPtr = std::shared_ptr<DrawableNodeProxy>;
    class DrawableNode : public Node<DrawableNode> {
    public:
        DrawableNode() = default;
        explicit DrawableNode(const std::vector<std::shared_ptr<GRGLMesh>>& mesh);
        ~DrawableNode() = default;

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

        StateSetDummy getAttribute() const
        {
            return m_attribute;
        }

        void attach(NodeProxyPtr pNode);
        void detach(NodeProxyPtr pNode);

        bool dirty() const
        {
            return m_nodeDirty;
        }

        void setDirty(bool hint)
        {
            m_nodeDirty = hint;
        }

        GRUINT32 observeNum() const
        {
            return m_observer.size();
        }

        NodeProxyPtr getObserver(GRUINT32 index) const
        {
            return m_observer[index].lock();
        }

        std::vector<std::weak_ptr<GRGLMesh>> geoDatas() const
        {
            return m_geoDatas;
        }
    private:
        GeometryDataDummy m_data;
        StateSetDummy m_attribute;
        std::vector<NodeBase*> m_parentList;
        std::vector<NodePtr> m_childList;

        std::vector<std::weak_ptr<DrawableNodeProxy>> m_observer;
        std::vector<std::weak_ptr<GRGLMesh>> m_geoDatas;
        bool m_nodeDirty = false; //node是否需要更新，或者node是否为新
    };
}
#endif // !_DRAWABLE_NODE_H
