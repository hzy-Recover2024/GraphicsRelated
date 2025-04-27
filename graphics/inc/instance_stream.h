/*定义vertex stream 的instance 属性*/

#ifndef _INSTANCE_STREAM_H
#define _INSTANCE_STREAM_H
#include "grgl_object.h"
#include <vector>
#include <map>
namespace GRelated {
    class GLFWGLContext;
    class DrawableNode;
    class DrawableNodeProxy;
    class InstanceStream {
    public:
        void addNode(DrawableNodeProxy* pNode);
        void uploadInstace(GLFWGLContext* pContext);

    private:
        bool m_needUpdate = false;
        std::map<DrawableNode*, std::vector<DrawableNodeProxy*>> m_nodeLayout;
        GRUINT32 m_insCount = 0;
        std::weak_ptr<GRGLBufferObject> m_insvbo;
        std::weak_ptr<GRGLBufferObject> m_matSSBO;
    };
    
}

#endif // !_INSTANCE_STREAM_H

