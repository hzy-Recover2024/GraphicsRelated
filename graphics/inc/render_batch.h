/*定义单次drawcall的batched primitive数据*/

#ifndef _RENDER_BATCH_H
#define _RENDER_BATCH_H
#include <memory>
#include "grgl_vertex_rendering.h"
namespace GRelated {
    class GRGLVertexArrayObject;
    class GLFWGLContext;
    class InstanceStream;
    class VertexStream;
    class RenderBatch {
    public:
        virtual ~RenderBatch() = default;
    };
    class TriangleBatch : public RenderBatch {
    public:
        void buildCommand(GLFWGLContext&, InstanceStream&, VertexStream&);
        void draw();
    private:
        bool m_setUp = false;
        std::unique_ptr<GRGLTriangleRendering> m_cmd;
        std::weak_ptr<GRGLVertexArrayObject> m_vao;
    };
    
}

#endif // !_RENDER_BATCH_H

