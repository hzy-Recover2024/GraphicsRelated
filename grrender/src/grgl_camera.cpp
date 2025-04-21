#include "grgl_vertex_pipeline.h"
#include <string>
#include <fstream>
#include <sstream>
#include <assert.h>
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
    }

}

