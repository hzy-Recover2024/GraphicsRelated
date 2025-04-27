#include "vertex_stream.h"
#include "grgl_context.h"
#include "drawable_node.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace GRelated {
    namespace {
        struct alignas(sizeof(glm::vec3::value_type)) VertexNormal
        {
            glm::vec3 pos;
            glm::vec3 normal;
        };
    }

    void VertexStream::addNode(const DrawableNode* pNode)
    {
        m_nodelist.push_back({ pNode });
        m_vertexCountProbably += pNode->vertexCount();
        m_loactionMap[pNode] = m_nodelist.size() - 1;
    }

    void VertexStream::uploadVertex(GLFWGLContext* pContext)
    {
        if (!m_needUpade)
        {
            return;
        }
        m_needUpade = true;
        long long vertexsize = static_cast<long long>(m_vertexCountProbably * sizeof(VertexNormal) * 1.2);
        auto newVbo = GRGLBufferObject::create(*pContext, ObjectTarget::kVBO,
            static_cast<GRGL_sizeiptr>(vertexsize)
            , nullptr, true);
        newVbo->registerContext();
        long long indexSize = static_cast<long long>(m_vertexCountProbably * sizeof(GRGL_uint) * 1.2);
        m_vbo = newVbo;
        auto newEbo = GRGLBufferObject::create(*pContext, ObjectTarget::kEBO,
            static_cast<GRGL_sizeiptr>(m_vertexCountProbably * sizeof(GRGL_uint) * 1.2)
            , nullptr, true);
        newEbo->registerContext();
        m_ebo = newEbo;

        // map and upload data
        newVbo->bind();
        auto vertexPtr = reinterpret_cast<VertexNormal*>(newVbo->map(0,
            static_cast<GRGL_sizeiptr>(vertexsize)));

        newEbo->bind();
        auto indexPtr = reinterpret_cast<GRGL_uint*>(newVbo->map(0,
            static_cast<GRGL_sizeiptr>(indexSize)));
        GRUINT32 vertexOffset = 0;
        GRUINT32 indexOffset = 0;
        for (int i = 0; i < m_nodelist.size(); ++i)
        {
            m_nodelist[i].vertexOffset = vertexOffset;
            m_nodelist[i].indexOffset = indexOffset;
            GRUINT32 meshindexOffset = 0;
            auto meshList = m_nodelist[i].dn->geoDatas();
            for (const auto& mesh : meshList)
            {
                auto meshPtr = mesh.lock();
                if (meshPtr == nullptr)
                {
                    continue;
                }
                for (auto index : meshPtr->indices())
                {
                    *indexPtr = index + meshindexOffset;
                    ++indexPtr;
                    indexOffset++;
                }
                for (auto vertex : meshPtr->vertices())
                {
                    *vertexPtr = { vertex.Position, vertex.Normal };
                    ++vertexPtr;
                    meshindexOffset++;
                    vertexOffset++;
                }
                meshPtr->releaseData();
            }
        }
        newVbo->unmap();
        newEbo->unmap();
        newVbo->unbind();
        newEbo->unbind();
    }

    GRUINT32 VertexStream::getVertexOffset(const DrawableNode* pNode)
    {
        auto itr = m_loactionMap.find(pNode);
        if (itr == m_loactionMap.end())
        {
            return 0;
        }
        return m_nodelist[itr->second].vertexOffset;
    }

    GRUINT32 VertexStream::getIndexOffset(const DrawableNode* pNode)
    {
        auto itr = m_loactionMap.find(pNode);
        if (itr == m_loactionMap.end())
        {
            return 0;
        }
        return m_nodelist[itr->second].indexOffset;
    }

    void VertexStream::setupVertexFormat(
        std::shared_ptr<GRGLVertexArrayObject> vao)
    {
        if (vao == nullptr)
        {
            return;
        }
        vao->bind();
        auto vbo = m_vbo.lock();
        if (vbo)
        {
            vbo->bind();
        }
        vao->enableVertexAttribArray(0);
        vao->enableVertexAttribArray(1);
        vao->vertexAttriPointer(0, 3, TypeEnum::kFLOAT,
            false, sizeof(VertexNormal), offsetof(VertexNormal, pos));
        vao->vertexAttriPointer(1, 3, TypeEnum::kFLOAT, false,
            sizeof(VertexNormal), offsetof(VertexNormal, normal));
        auto ebo = m_ebo.lock();
        if (ebo)
        {
            ebo->bind();
        }
        vao->unbind();
        ebo->unbind();
        vbo->unbind();
    }

}

