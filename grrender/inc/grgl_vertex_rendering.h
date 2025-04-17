/*定义vertex rendering，包括各种图元的绘制指令*/

#ifndef _GRGL_VERTEX_RENDERING_H
#define _GRGL_VERTEX_RENDERING_H

#include "grgl_object.h"
#include <memory>
namespace GRelated {
    struct alignas(sizeof(GRGL_uint)) DrawArraysIndirectCommand
    {
        GRGL_uint count;
        GRGL_uint instanceCount;
        GRGL_uint first;
        GRGL_uint baseInstance;
    };
    
    struct alignas(sizeof(GRGL_uint)) DrawElementsIndirectCommand
    {
        GRGL_uint  count;
        GRGL_uint  instanceCount;
        GRGL_uint  firstIndex;
        GRGL_uint  baseVertex;
        GRGL_uint  baseInstance;
    };

    class GRGLVertexRendering {
    public:
        GRGLVertexRendering() {}
        virtual ~GRGLVertexRendering() {}

    protected:
        std::weak_ptr<GRGLBufferObjectPersistMapped> m_DIBO;
    };

    class GRGLPointRendering : public GRGLVertexRendering {
    public:
        GRGLPointRendering() {}
        ~GRGLPointRendering() override {}

        void drawPoint(){}

    };

    class GRGLLinesRendering : public GRGLVertexRendering {
    public:
        GRGLLinesRendering() {}
        ~GRGLLinesRendering() override {}

        void drawLines() {}
        void drawLineStrip() {}
        void drawLineLoop() {}

        void setPrimitiveRestart(GRGL_uint index);
    };

    class GRGLTriangleRendering : public GRGLVertexRendering {
    public:
        GRGLTriangleRendering() {}
        ~GRGLTriangleRendering() override {}

        void drawTriangle() {}
        void drawTriangleStrip() {}

        void setPrimitiveRestart(GRGL_uint index);
    };
}

#endif // !_GRGL_VERTEX_RENDERING_H

