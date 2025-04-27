#include "entry.h"
#include "glad/glad.h"
#include "GLFW//glfw3.h"
#include <assert.h>
#include <iostream>
namespace GRelated {
    namespace {
        // GLFW Error
        void error_callback(int error, const char* description)
        {
            std::cerr << description << "\n";
            assert(0);
        }

    }
    Entry::Entry()
    {
        std::cout << "error";
        glfwSetErrorCallback(error_callback);
        if (!glfwInit())
        {
            // Initialization failed
            assert(0);
        }
    }
    Entry::~Entry()
    {
        glfwTerminate();
    }
}


