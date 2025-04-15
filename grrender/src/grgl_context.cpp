#include "grgl_context.h"
#include "glad/glad.h"
#include "GLFW//glfw3.h"
#include <assert.h>
namespace GRelated {

    namespace {
        void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    GRGLContext::GRGLContext(ContextInfo contextType)
        : m_contextType(contextType)
    {

    }

    void GLFWGLContext::grglFinish()
    {
        ::glFinish();
    }

    void GLFWGLContext::grglFlush()
    {
        ::glFlush();
    }

    void GLFWGLContext::grglEnable(GRGL_enum cap)
    {
        ::glEnable(cap);
    }

    void GLFWGLContext::grglDisable(GRGL_enum cap)
    {
        ::glDisable(cap);
    }

    void GLFWGLContext::detachObject(GRGL_enum target, GRGLObject* pObj)
    {
        auto objType = pObj->type();
        switch (objType)
        {
        case GRelated::ObjectType::kBuffer:
        {
            if (m_bufferBoundMap[target] == pObj)
            {
                ::glBindBuffer(target, 0);
            }
        }
            break;
        case GRelated::ObjectType::kTexture:
        {
            if (m_textureBoundMap[target] == pObj)
            {
                ::glBindTexture(target, 0);
            }
        }
            break;
        case GRelated::ObjectType::kQuery:
            break;
        case GRelated::ObjectType::kFrameBuffer:
        {
            if (pObj == m_drawFramebufferObj)
            {
                ::glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            }
            else if (pObj == m_readFramebufferObj)
            {
                ::glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
            }
        }
            break;
        case GRelated::ObjectType::kRenderBuffer:
        {
            if (m_renderbufferObj == pObj)
            {
                ::glBindRenderbuffer(GL_RENDERBUFFER, 0);
            }
        }
            break;
        case GRelated::ObjectType::kSampler:
            break;
        case GRelated::ObjectType::kVAO:
            break;
        default:
            break;
        }
    }

    void GLFWGLContext::attachObject(GRGL_enum target, GRGLObject* pObj)
    {
        auto objType = pObj->type();
        switch (objType)
        {
        case GRelated::ObjectType::kBuffer:
            m_bufferBoundMap[target] = pObj;
            break;
        case GRelated::ObjectType::kTexture:
            m_textureBoundMap[target] = pObj;
            break;
        case GRelated::ObjectType::kQuery:
            break;
        case GRelated::ObjectType::kFrameBuffer:
        {
            if (target == GL_DRAW_FRAMEBUFFER)
            {
                m_drawFramebufferObj = pObj;
            }
            else if (target == GL_READ_FRAMEBUFFER)
            {
                m_readFramebufferObj = pObj;
            }
        }
            break;
        case GRelated::ObjectType::kRenderBuffer:
            m_renderbufferObj = pObj;
            break;
        case GRelated::ObjectType::kSampler:
            break;
        case GRelated::ObjectType::kVAO:
            break;
        default:
            break;
        }
    }

    void GLFWGLContext::record(ObjectPtr ptr)
    {
        m_objects.insert(ptr);
    }

    void GLFWGLContext::release(ObjectPtr ptr)
    {
        m_objects.erase(ptr);
    }

    void GLFWGLContext::grglSetDefaultFramebuffer(FBOType type)
    {
        if (type == kDraw)
        {
            ::glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        }
        else if (type == kRead)
        {
            ::glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        }
    }

    void GLFWGLContext::grglBlitFrameBuffer(GRGL_int srcX0, GRGL_int srcY0,
        GRGL_int srcX1, GRGL_int srcY1,
        GRGL_int dstX0, GRGL_int dstY0,
        GRGL_int dstX1, GRGL_int dstY1, std::bitset<8> mask)
    {
        GRGL_uint transMask = 0;
        if (mask.test(static_cast<size_t>(FBInnerBuffer::kColor)))
        {
            transMask |= GL_COLOR_BUFFER_BIT;
        }
        if (mask.test(static_cast<size_t>(FBInnerBuffer::kDepth)))
        {
            transMask |= GL_DEPTH_BUFFER_BIT;
        }
        if (mask.test(static_cast<size_t>(FBInnerBuffer::kStencil)))
        {
            transMask |= GL_STENCIL_BUFFER_BIT;
        }
        ::glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1,
            transMask, GL_LINEAR);
    }

    void GLFWGLContext::clearColoriv(GRGL_sizei index, const std::array<GRGL_int, 4>& value)
    {
        ::glClearBufferiv(GL_COLOR, GL_DRAW_BUFFER0 + index, value.data());
    }

    void GLFWGLContext::clearColoruiv(GRGL_sizei index, const std::array<GRGL_uint, 4>& value)
    {
        ::glClearBufferuiv(GL_COLOR, GL_DRAW_BUFFER0 + index, value.data());
    }

    void GLFWGLContext::clearColorfv(GRGL_sizei index, const std::array<GRGL_float, 4>& value)
    {
        ::glClearBufferfv(GL_COLOR, GL_DRAW_BUFFER0 + index, value.data());
    }

    void GLFWGLContext::clearDepth(GRGL_float value)
    {
        ::glClearBufferfv(GL_DEPTH, 0, &value);
    }

    void GLFWGLContext::_init()
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

    GLFWGLContext::GLFWGLContext(ContextInfo contextType,
        GRGL_int majorVersion, GRGL_int minorVersion,
        GRGL_uint width, GRGL_uint height)
        : GRGLContext(contextType)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
        if (contextType == GRGLContext::ContextInfo::kCore)
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

