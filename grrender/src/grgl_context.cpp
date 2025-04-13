#include "grgl_context.h"
#include "glad/glad.h"
#include "GLFW//glfw3.h"
#include <assert.h>
namespace GRelated {

    GRGLContext::GRGLContext(ContextInfo contextType)
        : m_contextType(contextType)
    {

    }

    void GRGLContext::grglFinish()
    {
        ::glFinish();
    }

    void GRGLContext::grglFlush()
    {
        ::glFlush();
    }

    void GRGLContext::grglEnable(GRGL_enum cap)
    {
        ::glEnable(cap);
    }

    void GRGLContext::grglDisable(GRGL_enum cap)
    {
        ::glDisable(cap);
    }

    void GRGLContext::_init()
    {
        m_glslVersion = reinterpret_cast<const char*>(
            ::glGetString(GL_SHADING_LANGUAGE_VERSION));
        m_renderer = reinterpret_cast<const char*>(
            ::glGetString(GL_RENDERER));
        m_vendor = reinterpret_cast<const char*>(
            ::glGetString(GL_VENDOR));
        m_version = reinterpret_cast<const char*>(
            ::glGetString(GL_VERSION));
    }
    namespace {
        void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    GLFWGLContext::GLFWGLContext(ContextInfo contextType,
        GRGL_int majorVersion, GRGL_int minorVersion,
        GRGL_uint width, GRGL_uint height)
        : GRGLContext(contextType)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
        if (contextType == GRGLContext::kCore)
        {
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }
        else
        {
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
        }
        GLFWwindow* window = glfwCreateWindow(width, height, "GRGL_Window", NULL, NULL);
        if (!window)
        {
            // Window or context creation failed
            assert(0);
            terminate();
        }
        glfwSetKeyCallback(window, key_callback);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);
        int version = gladLoadGL();
        if (version == 0) {
            assert(0);
            terminate();
        }
        int fbWidth = 0;
        int fbHeight = 0;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight); // 获取物理像素
        m_fbWidth = static_cast<GRGL_uint>(fbWidth);
        m_fbHeight = static_cast<GRGL_uint>(fbHeight);
        _init();
    }

    GLFWGLContext::~GLFWGLContext()
    {
        glfwDestroyWindow(static_cast<GLFWwindow*>(m_window));
    }

    void GLFWGLContext::makeCurrent()
    {
        glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_window));
    }

    bool GLFWGLContext::shouldClose()
    {
        return glfwWindowShouldClose(static_cast<GLFWwindow*>(m_window));
    }

    void GLFWGLContext::swapBuffers()
    {
        ::glfwSwapBuffers(static_cast<GLFWwindow*>(m_window));
    }

    void GLFWGLContext::pollEvents()
    {
        ::glfwPollEvents();
    }

}

