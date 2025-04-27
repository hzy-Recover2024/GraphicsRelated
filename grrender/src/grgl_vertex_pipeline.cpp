#include "grgl_vertex_pipeline.h"
#include <string>
#include <fstream>
#include <sstream>
#include <assert.h>
#include "grgl_context.h"
#include <filesystem>
#include <windows.h>
#include <iostream>
namespace GRelated {
    namespace {
        std::string loadShader(const char* shaderPath)
        {
            std::ifstream shaderFile;
            shaderFile.open(shaderPath);
            if (!shaderFile.is_open())
            {
                assert(0);
                return {};
            }
            std::stringstream shaderStream;
            // read file's buffer contents into streams
            shaderStream << shaderFile.rdbuf();
            // close file handlers
            shaderFile.close();
            // convert stream into string
            return shaderStream.str();
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
                std::cerr << "´íÎó: " << e.what() << std::endl;
                return {};
            }
        }

    }

    void GRGLVertexPipeline::buildPipeline(GRGLContext& context)
    {
        if (m_meshPhong != nullptr)
        {
            return;
        }
        std::filesystem::path exe_dir = get_executable_dir();
        auto shader_vert = GRGLShader::create(context, ShaderType::kVertexShader,
            loadShader((exe_dir / "shader" / "multiple_lights.vs").string().c_str()));
        auto shader_fs = GRGLShader::create(context, ShaderType::kFragment_shader,
            loadShader((exe_dir / "shader" / "multiple_lights.fs").string().c_str()));
        shader_vert->registerContext();
        shader_fs->registerContext();
        shader_vert->compile();
        shader_fs->compile();
        m_meshPhong = GRGLProgram::create(context);
        m_meshPhong->attachShader(shader_vert);
        m_meshPhong->attachShader(shader_fs);
        m_meshPhong->link();
    }


    GRelated::GRGLProgram& GRGLVertexPipeline::meshPhong()
    {
        assert(m_meshPhong != nullptr);
        return *m_meshPhong;
    }

}

