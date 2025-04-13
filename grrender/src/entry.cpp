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

        class Entry {
        public:
            Entry()
            {
                glfwSetErrorCallback(error_callback);
                if (!glfwInit())
                {
                    // Initialization failed
                    assert(0);
                }
            }

            ~Entry()
            {
                glfwTerminate();
            }
        };

        const Entry init;
    }

}

