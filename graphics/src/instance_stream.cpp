#include "instance_stream.h"
#include "grgl_context.h"
#include "drawable_node.h"
#include "drawable_node_proxy.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace GRelated {
    namespace {
        struct alignas(sizeof(glm::vec3::value_type)) InstanceData
        {
            glm::vec4 color;
        };

        struct alignas(sizeof(glm::mat4::value_type)) MatData
        {
            glm::mat4 mat;
        };

    }

    void InstanceStream::addNode(DrawableNodeProxy* pNode)
    {
        m_nodeLayout[pNode->associatedNode()].push_back(pNode);
        m_insCount++;
    }

    void InstanceStream::uploadInstace(GLFWGLContext* pContext)
    {
        if (!m_needUpdate)
        {
            return;
        }
        m_needUpdate = false;
        auto insSize = (long long)(m_insCount * sizeof(InstanceData) * 1.2);
        auto newVBO = GRGLBufferObject::create(*pContext, ObjectTarget::kVBO,
            static_cast<GRGL_sizeiptr>(insSize),
            nullptr, true
        );
        m_insvbo = newVBO;

        auto ssboSize = (long long)(m_insCount * sizeof(MatData) * 1.2);
        auto newSSBO = GRGLBufferObject::create(*pContext, ObjectTarget::kSSBO,
            static_cast<GRGL_sizeiptr>(ssboSize),
            nullptr, true
        );
        m_matSSBO = newSSBO;
        newVBO->bind();
        auto pinsData = reinterpret_cast<InstanceData*>(newVBO->map(0, static_cast<GRGL_sizeiptr>(insSize)));
        newSSBO->bind();
        auto pmatData = reinterpret_cast<MatData*>(newSSBO->map(0, static_cast<GRGL_sizeiptr>(ssboSize)));

        int insIndex = 0;
        for (const auto& nodes : m_nodeLayout)
        {
            for (const auto& node : nodes.second)
            {
                pinsData->color = glm::vec4(node->color(), (glm::vec4::value_type)insIndex);
                pmatData->mat = node->mat();
                pinsData++;
                pmatData++;
                insIndex++;
            }
        }
        newVBO->unmap();
        newSSBO->unmap();
        newVBO->unbind();
        newSSBO->unbind();
    }

}

