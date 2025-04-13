/*opengl context*/

#ifndef _GRGL_CONTEXT_H
#define _GRGL_CONTEXT_H
#include <string>
#include "grgl_type.h"
namespace GRelated {
    class GRGLContext {
    public:
        enum ContextInfo
        {
            kCore,
            kCompatibility,
        };

        explicit GRGLContext(ContextInfo contextType);

        virtual ~GRGLContext()
        {

        }

        virtual void makeCurrent() = 0;

        ContextInfo contextType() const
        {
            return m_contextType;
        }
        std::string contextVersion() const
        {
            return m_version;
        }
        std::string vendor() const
        {
            return m_vendor;
        }
        std::string renderer() const
        {
            return m_renderer;
        }
        std::string GLSLVersion() const
        {
            return m_glslVersion;
        }

        void grglFinish();
        void grglFlush();

        void grglEnable(GRGL_enum cap);
        void grglDisable(GRGL_enum cap);

    protected:
        void _init();

    protected:
        // context的一些信息
        std::string m_version;
        std::string m_vendor;
        std::string m_renderer;
        std::string m_glslVersion;
        ContextInfo m_contextType;

    };

    // 使用GLFW创建窗口并设置对应OpenGL context
    class GLFWGLContext : public GRGLContext {
    public:
        GLFWGLContext(ContextInfo contextType,
            GRGL_int majorVersion, GRGL_int minorVersion,
            GRGL_uint width, GRGL_uint height);
        ~GLFWGLContext();

        void makeCurrent() override;
        bool shouldClose();
        void swapBuffers();
        void pollEvents();
        GRGL_uint fbWidth() const
        {
            return m_fbWidth;
        }
        GRGL_uint fbHeight() const
        {
            return m_fbHeight;
        }
    private:
        void* m_window;
        GRGL_uint m_fbWidth = 0;
        GRGL_uint m_fbHeight = 0;
    };
}

#endif // !_GRGL_CONTEXT_H

