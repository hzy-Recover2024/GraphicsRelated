#include "grgl_vertex_rendering.h"
#include "grgl_context.h"
#include "glad/glad.h"
#include "GLFW//glfw3.h"
#include "grgl_object.h"
namespace GRelated {

    GRGLTriangleRendering::GRGLTriangleRendering(GRGLContext& ctx)
        : GRGLVertexRendering(ctx)
    {

    }


    void GRGLTriangleRendering::uploadCommand(std::vector<DrawElementsIndirectCommand>& cmdList)
    {
        if (!m_DIBO.expired())
        {
            return;
        }
        m_cmdNum = cmdList.size();
        auto cmdSize = (GRGL_sizeiptr)(m_cmdNum * sizeof(DrawElementsIndirectCommand) * 1.2f);

        auto newDibo = GRGLBufferObject::create(m_context, ObjectTarget::kDIBO,
            cmdSize, nullptr, true
        );
        newDibo->registerContext();
        m_DIBO = newDibo;
        newDibo->bind();
        auto pCmd = reinterpret_cast<DrawElementsIndirectCommand*>(newDibo->map(0, cmdSize));
        ::memcpy(pCmd, cmdList.data(), m_cmdNum * sizeof(DrawElementsIndirectCommand));
        newDibo->unmap();
        newDibo->unbind();
    }

    void GRGLTriangleRendering::drawTriangle()
    {
        auto dibo = m_DIBO.lock();
        if (dibo)
        {
            dibo->bind();
        }
        else
        {
            return;
        }
        ::glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, m_cmdNum, 0);
        dibo->unbind();
    }

}

