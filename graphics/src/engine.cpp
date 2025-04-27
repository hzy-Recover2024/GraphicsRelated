#include "engine.h"
#include <string>
#include <filesystem>
#include <stdexcept>
#ifdef _WIN32
#include <windows.h>
#endif
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "transform_node.h"
#include "view.h"

#include "grgl_context.h"
#include "grgl_vertex_pipeline.h"
#include "grgl_object.h"

namespace GRelated {
    namespace {
        glm::vec3 getTranslate()
        {
            float x = rand() % 100 - 50;
            float y = rand() % 100 - 50;
            float z = rand() % 100 - 50;
            return glm::vec3(x, y, z);
        }

        glm::vec4 getRoateDir()
        {
            float x = rand() % 100 - 50;
            float y = rand() % 100 - 50;
            float z = rand() % 100 - 50;
            float tmp_radians = glm::radians(rand() / 360.f);
            glm::vec3 dir(x, y, z);
            return { glm::normalize(dir), tmp_radians };
        }

        std::filesystem::path get_executable_dir() {
            try {
#ifdef _WIN32
                wchar_t path[MAX_PATH] = { 0 };
                if (GetModuleFileNameW(nullptr, path, MAX_PATH) == 0) {
                    throw std::runtime_error("GetModuleFileNameW failed");
                }
                return std::filesystem::path(path).parent_path();
#else
                return fs::canonical("/proc/self/exe").parent_path();
#endif
            }
            catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "错误: " << e.what() << std::endl;
                return {};
            }
        }
    }

    void Engine::draw()
    {
        initContext();
        initSecne();

        while (!m_context->shouldClose())
        {
            // UI callback
            m_context->pollEvents();
            m_context->makeCurrent();
            // some change of scene
            // .....
            //
            // update scene
            //.....
            //

            // prepare vertexStream
            uploadVertexData();
            // prepare instanceStream
            View::getInstance().uploadInstance(m_context.get());

            // 更新相机
            View::getInstance().updateCamera(m_context.get());

            // start rendering
            // pass1 绘制不透明物体，开启深度测试
            m_canvas->prepare(*m_context);
            
            m_depthTest->apply();
            m_opaque->apply();

            GRGLVertexPipeline::getInstance().buildPipeline(*m_context);
            GRGLVertexPipeline::getInstance().meshPhong().use();

            m_triBatch->buildCommand(*m_context, View::getInstance().instanceStream(), *m_vertexData);
            m_triBatch->draw();

            m_context->swapBuffers();
        }


    }

    void Engine::initContext()
    {
        if (m_context == nullptr)
        {
            return;
        }
        m_context = std::make_unique<GLFWGLContext>(GRGLContext::ContextInfo::kCore,
            4, 5, 1920, 1080);
        m_context->initGlobalState();
        m_context->makeCurrent();
        if (m_canvas == nullptr)
        {
            m_canvas = std::make_unique<Canvas>();
        }
        if (m_triBatch == nullptr)
        {
            m_triBatch = std::make_unique<TriangleBatch>();
        }
        if (m_depthTest == nullptr)
        {
            m_depthTest = std::make_unique<GRGLDepthTestStrategy>(*m_context);
            m_depthTest->setDepthTest(true);
            m_depthTest->setDetphTestFunc(GlobalState::kLESS);
        }
        if (m_opaque == nullptr)
        {
            m_opaque = std::make_unique<GRGLBlendStrategy>(*m_context);
            m_opaque->setDrawBufferBlend(0, false);
        }
    }

    void Engine::initSecne()
    {
        if (m_root.size() == m_models.size())
        {
            return;
        }
        for (const auto& model : m_models)
        {
            m_root.emplace_back(std::make_unique<DrawableNode>());
            View::getInstance().ensureInDrityList(m_root.back().get());
        }
        // 每个node进行6次矩阵变换，生成6个实例
        for (auto& root : m_root)
        {
            auto index = rand() % m_root.size() - 1;
            if (index < 0 || index == m_models.size())
            {
                index = 0;
            }
            auto newDN = std::make_shared<DrawableNode>(m_models[index].m_meshes);
            for (int i = 0; i < 6; ++i)
            {
                glm::mat4 mat(1.0f);
                mat = glm::translate(mat, getTranslate());
                auto roateDir = getRoateDir();
                mat = glm::rotate(mat, roateDir.w, glm::vec3(roateDir));
                auto transNode = std::make_shared<TransformNode>(mat);
                root->addChild(transNode);
                transNode->addParent(root.get());
                transNode->addChild(newDN);
                newDN->addParent(transNode.get());
            }
        }
        View::getInstance().buildScene();

    }

    void Engine::uploadVertexData()
    {
        if (m_vertexData != nullptr)
        {
            return;
        }
        m_vertexData = std::make_unique<VertexStream>();
        std::vector<NodeBase*> nodeSk;
        for (const auto& root : m_root)
        {
            nodeSk.push_back(root.get());
            while (!nodeSk.empty())
            {
                auto curNode = nodeSk.back();
                nodeSk.pop_back();
                if (curNode->nodeType() == NodeBase::kDrawable)
                {
                    m_vertexData->addNode(static_cast<DrawableNode*>(curNode));
                }
                for (GRUINT32 i = 0; i < curNode->childNum(); ++i)
                {
                    nodeSk.push_back(curNode->getChild(i).get());
                }
            }
        }
        m_vertexData->uploadVertex(m_context.get());
    }

    Engine::Engine()
    {
        std::filesystem::path exe_dir = get_executable_dir();
        m_models.emplace_back(
            (exe_dir / "resource" / "objects" / "backpack" / "backpack.obj").string());
        m_models.emplace_back(
            (exe_dir / "resource" / "objects" / "cyborg" / "cyborg.obj").string());
        m_models.emplace_back(
            (exe_dir / "resource" / "objects" / "nanosuit" / "nanosuit.obj").string());
        m_models.emplace_back(
            (exe_dir / "resource" / "objects" / "planet" / "planet.obj").string());
        m_models.emplace_back(
            (exe_dir / "resource" / "objects" / "rock" / "rock.obj").string());
    }

}