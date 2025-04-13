/*opengl context*/

#ifndef _GRGL_CONTEXT_H
#define _GRGL_CONTEXT_H
#include <string>
#include "grgl_type.h"
#include "grgl_object.h"
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

        
        virtual void detachObject(GRGL_enum target, GRGLGenericObject* pObj) = 0;
        virtual void attachObject(GRGL_enum target, GRGLGenericObject* pObj) = 0;

        virtual void grglFinish() = 0;
        virtual void grglFlush() = 0;

        virtual void grglEnable(GRGL_enum cap) = 0;
        virtual void grglDisable(GRGL_enum cap) = 0;

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

        // 窗口相关的函数
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

        // opengl 相关函数
        void grglFinish() override;
        void grglFlush() override;

        void grglEnable(GRGL_enum cap) override;
        void grglDisable(GRGL_enum cap) override;
    private:
        void _init();
    private:
        void* m_window;
        GRGL_uint m_fbWidth = 0;
        GRGL_uint m_fbHeight = 0;
    };
}

#endif // !_GRGL_CONTEXT_H

