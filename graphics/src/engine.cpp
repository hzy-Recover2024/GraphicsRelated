#include "engine.h"
#include <string>
#include "grgl_context.h"
#include <filesystem>
#include <stdexcept>
#ifdef _WIN32
#include <windows.h>
#endif
namespace GRelated {
    namespace {
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
                std::cerr << "´íÎó: " << e.what() << std::endl;
                return {};
            }
        }
    }

    void Engine::draw()
    {
        initContext();
        initSecne();
        
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
    }

    void Engine::initSecne()
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