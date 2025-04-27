/*定义vertex rendering，包括各种图元的绘制指令*/

#ifndef _GRGL_VERTEX_RENDERING_H
#define _GRGL_VERTEX_RENDERING_H

#include "grgl_object.h"
#include <vector>
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
    class GRGLContext;
    class GRGLVertexRendering {
    public:
        explicit GRGLVertexRendering(GRGLContext& ctx) : m_context(ctx) {}
        virtual ~GRGLVertexRendering() {}

    protected:
        GRGLContext& m_context;
        std::weak_ptr<GRGLBufferObject> m_DIBO;
    };

    class GRGLPointRendering : public GRGLVertexRendering {
    public:
        explicit GRGLPointRendering(GRGLContext& ctx);
        ~GRGLPointRendering() override {}

        void drawPoint(){}

    };

    class GRGLLinesRendering : public GRGLVertexRendering {
    public:
        explicit GRGLLinesRendering(GRGLContext& ctx);
        ~GRGLLinesRendering() override {}

        void drawLines() {}
        void drawLineStrip() {}
        void drawLineLoop() {}

        void setPrimitiveRestart(GRGL_uint index);
    };

    class GRGLTriangleRendering : public GRGLVertexRendering {
    public:
        explicit GRGLTriangleRendering(GRGLContext& ctx);
        ~GRGLTriangleRendering() override {}

        void uploadCommand(std::vector<DrawElementsIndirectCommand>& cmdList);
        DrawElementsIndirectCommand* dataPointer();
        void drawTriangle();
        void drawTriangleStrip() {}

        void setPrimitiveRestart(GRGL_uint index);

        void setCmdNum(GRGL_uint num)
        {
            m_cmdNum = num;
        }

    private:
        GRGL_uint m_cmdNum = 0;
    };
}

#endif // !_GRGL_VERTEX_RENDERING_H

