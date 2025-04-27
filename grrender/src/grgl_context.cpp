#include "grgl_context.h"
#include "glad/glad.h"
#include "GLFW//glfw3.h"

#include <assert.h>
namespace GRelated {

    GRGL_enum transGlobalState(GlobalState state)
    {
        switch (state)
        {
#define ENUM_MAP(NAME) case GlobalState::k##NAME: return GL_##NAME;
            GLOBAL_STATE_ENUM
#undef ENUM_MAP
        default:
            break;
        }
        assert(0);
        return 0;
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

    void GLFWGLContext::initGlobalState()
    {
        // 开启点图元的点大小，并设置点片元的相对坐标
        ::glEnable(GL_PROGRAM_POINT_SIZE);
        ::glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT); // 左下角为原点
        ::glEnable(GL_SCISSOR_TEST);
        grglViewport(0, 0, m_fbWidth, m_fbHeight);
        grglDepthRangef(0.f, 1.0f);
    }

    void GLFWGLContext::grglViewport(GRGL_int x, GRGL_int y, GRGL_sizei width, GRGL_sizei height)
    {
        ::glViewport(x, y, width, height);
        ::glScissor(x, y, width, height);
    }

    void GLFWGLContext::grglDepthRangef(GRGL_float nearVal, GRGL_float farVal)
    {
        ::glDepthRangef(nearVal, farVal);
    }

    void GLFWGLContext::grglFrontFace(GRGL_enum mode)
    {
        ::glFrontFace(mode);
    }

    void GLFWGLContext::grglCullFace(GRGL_enum mode)
    {
        ::glCullFace(mode);
    }

    void GLFWGLContext::grglStencilFuncSeparate(GRGL_enum face, GRGL_enum func, GRGL_int ref, GRGL_uint mask)
    {
        ::glStencilFuncSeparate(face, func, ref, mask);
    }

    void GLFWGLContext::grglStencilOpSeparate(GRGL_enum face, GRGL_enum sfail, GRGL_enum dpfail, GRGL_enum dppass)
    {
        ::glStencilOpSeparate(face, sfail, dpfail, dppass);
    }

    void GLFWGLContext::grglDepthFunc(GRGL_enum func)
    {
        ::glDepthFunc(func);
    }

    void GLFWGLContext::grglBlendEquationSeparate(GRGL_enum modeRGB, GRGL_enum modeAlpha)
    {
        ::glBlendEquationSeparate(modeRGB, modeAlpha);
    }

    void GLFWGLContext::grglBlendFuncSeparate(GRGL_enum srcRGB, GRGL_enum dstRGB, GRGL_enum srcAlpha, GRGL_enum dstAlpha)
    {
        ::glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
    }

    void GLFWGLContext::grglColorMaski(GRGL_uint buf, bool red, bool green, bool blue, bool alpha)
    {
        ::glColorMaski(buf, red, green, blue, alpha);
    }

    void GLFWGLContext::grglDepthMask(bool flag)
    {
        ::glDepthMask(flag);
    }

    void GLFWGLContext::grglEnablei(GRGL_enum cap, GRGL_uint index)
    {
        ::glEnablei(cap, index);
    }

    void GLFWGLContext::grglDisablei(GRGL_enum cap, GRGL_uint index)
    {
        ::glDisablei(cap, index);
    }

    void GLFWGLContext::registerkey_callback(void (*func)(GLFWwindow* window, int key, int scancode, int action, int mods))
    {
        glfwSetKeyCallback(static_cast<GLFWwindow*>(m_window), func);
    }

    void GLFWGLContext::registermouse_callback(void (*func)(GLFWwindow* window, double xposIn, double yposIn))
    {
        glfwSetCursorPosCallback(static_cast<GLFWwindow*>(m_window), func);
    }

    void GLFWGLContext::registerscroll_callback(void (*func)(GLFWwindow* window, double xoffset, double yoffset))
    {
        glfwSetScrollCallback(static_cast<GLFWwindow*>(m_window), func);
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

        // tell GLFW to capture our mouse
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwMakeContextCurrent(window);
        glfwSwapInterval(0);
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

