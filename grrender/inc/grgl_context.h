/*opengl context*/

#ifndef _GRGL_CONTEXT_H
#define _GRGL_CONTEXT_H
#include <array>
#include <map>
#include <set>
#include <string>
#include "grgl_type.h"
#include "grgl_object.h"
#include "grgl_texture.h"
#include "grgl_FBO.h"

namespace GRelated {
    class GRGLContext {
    public:
        enum class ContextInfo
        {
            kCore,
            kCompatibility,
        };

        explicit GRGLContext(ContextInfo contextType);

        virtual ~GRGLContext() = default;

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

        //设置一些全局性质的OpenGL状态
        virtual void initGlobalState() = 0;

        // object的绑定和管理
        virtual void detachObject(GRGL_enum target, GRGLObject* pObj) = 0;
        virtual void attachObject(GRGL_enum target, GRGLObject* pObj) = 0;
        virtual void record(ObjectPtr ptr) = 0;
        virtual void release(ObjectPtr ptr) = 0;

        // x,y代表视口左下角的原点
        virtual void grglViewport(GRGL_int x, GRGL_int y, GRGL_sizei width, GRGL_sizei height) = 0;
        virtual void grglDepthRangef(GRGL_float nearVal, GRGL_float farVal) = 0;

        // 背面剔除
        virtual void grglFrontFace(GRGL_enum mode) = 0;
        virtual void grglCullFace(GRGL_enum mode) = 0;

        // 片元测试
        virtual void grglStencilFuncSeparate(GRGL_enum face, GRGL_enum func, GRGL_int ref, GRGL_uint mask) = 0;
        virtual void grglStencilOpSeparate(GRGL_enum face, GRGL_enum sfail, GRGL_enum dpfail, GRGL_enum dppass) = 0;
        virtual void grglDepthFunc(GRGL_enum func) = 0;


        // 混合
        virtual void grglBlendEquationSeparate(GRGL_enum modeRGB, GRGL_enum modeAlpha) = 0;
        virtual void grglBlendFuncSeparate(GRGL_enum srcRGB, GRGL_enum dstRGB, GRGL_enum srcAlpha, GRGL_enum dstAlpha) = 0;

        // 颜色、深度写入控制
        virtual void grglColorMaski(GRGL_uint buf, bool red, bool green, bool blue, bool alpha) = 0;
        virtual void grglDepthMask(bool flag) = 0;

        virtual void grglFinish() = 0;
        virtual void grglFlush() = 0;

        virtual void grglEnable(GRGL_enum cap) = 0;
        virtual void grglDisable(GRGL_enum cap) = 0;


        // framebuffer相关函数
        enum class FBInnerBuffer : GRGL_byte {
            kColor,
            kDepth,
            kStencil
        };

        virtual void grglSetDefaultFramebuffer(FBOType type) = 0;
        virtual void grglBlitFrameBuffer(GRGL_int srcX0, GRGL_int srcY0,
                                         GRGL_int srcX1, GRGL_int srcY1,
                                         GRGL_int dstX0, GRGL_int dstY0,
                                         GRGL_int dstX1, GRGL_int dstY1,
                                         std::bitset<8> mask) = 0;
        virtual void clearColoriv(GRGL_sizei index, const std::array<GRGL_int, 4>& value) = 0;
        virtual void clearColoruiv(GRGL_sizei index, const std::array<GRGL_uint, 4>& value) = 0;
        virtual void clearColorfv(GRGL_sizei index, const std::array<GRGL_float, 4>& value) = 0;
        virtual void clearDepth(GRGL_float value) = 0;
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

        void detachObject(GRGL_enum target, GRGLObject* pObj) override;


        void attachObject(GRGL_enum target, GRGLObject* pObj) override;


        void record(ObjectPtr ptr) override;


        void release(ObjectPtr ptr) override;


        void grglSetDefaultFramebuffer(FBOType type) override;


        void grglBlitFrameBuffer(GRGL_int srcX0, GRGL_int srcY0,
            GRGL_int srcX1, GRGL_int srcY1,
            GRGL_int dstX0, GRGL_int dstY0,
            GRGL_int dstX1, GRGL_int dstY1, std::bitset<8> mask) override;


        void clearColoriv(GRGL_sizei index, const std::array<GRGL_int, 4>& value) override;

        void clearColoruiv(GRGL_sizei index, const std::array<GRGL_uint, 4>& value) override;

        void clearColorfv(GRGL_sizei index, const std::array<GRGL_float, 4>& value) override;

        void clearDepth(GRGL_float value) override;


        void initGlobalState() override;


        void grglViewport(GRGL_int x, GRGL_int y, GRGL_sizei width, GRGL_sizei height) override;


        void grglDepthRangef(GRGL_float nearVal, GRGL_float farVal) override;


        void grglFrontFace(GRGL_enum mode) override;


        void grglCullFace(GRGL_enum mode) override;


        void grglStencilFuncSeparate(GRGL_enum face, GRGL_enum func, GRGL_int ref, GRGL_uint mask) override;


        void grglStencilOpSeparate(GRGL_enum face, GRGL_enum sfail, GRGL_enum dpfail, GRGL_enum dppass) override;


        void grglDepthFunc(GRGL_enum func) override;


        void grglBlendEquationSeparate(GRGL_enum modeRGB, GRGL_enum modeAlpha) override;


        void grglBlendFuncSeparate(GRGL_enum srcRGB, GRGL_enum dstRGB, GRGL_enum srcAlpha, GRGL_enum dstAlpha) override;


        void grglColorMaski(GRGL_uint buf, bool red, bool green, bool blue, bool alpha) override;


        void grglDepthMask(bool flag) override;

    private:
        void _init();
    private:
        void* m_window;
        std::set<ObjectPtr> m_objects;
        std::map<GRGL_enum, GRGLObject*> m_bufferBoundMap;
        std::map<GRGL_enum, GRGLObject*> m_textureBoundMap;
        GRGLObject* m_drawFramebufferObj = nullptr;
        GRGLObject* m_readFramebufferObj = nullptr;
        GRGLObject* m_renderbufferObj = nullptr;
        GRGL_uint m_fbWidth = 0;
        GRGL_uint m_fbHeight = 0;
    };
}

#endif // !_GRGL_CONTEXT_H

