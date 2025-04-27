#include "instance_stream.h"
#include "grgl_context.h"
#include "render_batch.h"
#include "instance_stream.h"
#include "vertex_stream.h"
#include "grgl_object.h"
#include "grgl_vertex_rendering.h"
#include "drawable_node.h"
#include <vector>
namespace GRelated {

    void TriangleBatch::buildCommand(
        GLFWGLContext& contex, InstanceStream& insStream, VertexStream& vtxStream)
    {
        if (!m_vao.expired())
        {
            return;
        }
        auto vao = GRGLVertexArrayObject::create(contex);
        m_vao = vao;
        if (!m_setUp)
        {
            insStream.setupVertexFormat(vao);
            vtxStream.setupVertexFormat(vao);
            m_setUp = true;
        }
        std::vector<DrawElementsIndirectCommand> cmdlist;

        auto&& insLayout = insStream.instanceLayout();
        cmdlist.reserve(insLayout.size());
        GRGL_uint insIndex = 0;
        for (auto&& node : insLayout)
        {
            DrawElementsIndirectCommand cmd;
            cmd.baseInstance = insIndex;
            cmd.baseVertex = vtxStream.getVertexOffset(node.first);
            cmd.count = node.first->indexCount();
            cmd.firstIndex = vtxStream.getIndexOffset(node.first);
            cmd.instanceCount = node.second.size();
            insIndex += cmd.instanceCount;
            cmdlist.push_back(cmd);
        }
        if (m_cmd == nullptr)
        {
            m_cmd = std::make_unique<GRGLTriangleRendering>(contex);
        }
        m_cmd->uploadCommand(cmdlist);
    }

    void TriangleBatch::draw()
    {
        if (!m_setUp || m_cmd == nullptr)
        {
            assert(0);
            return;
        }
        auto vao = m_vao.lock();
        if (vao)
        {
            vao->bind();
            m_cmd->drawTriangle();
            vao->unbind();
        }
    }

}

