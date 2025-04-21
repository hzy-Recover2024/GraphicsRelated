/*定义模板测试、深度测试、混合的策略*/

#ifndef _GRGL_PER_SAMPLE_TEST_H
#define _GRGL_PER_SAMPLE_TEST_H

#include <grgl_type.h>
#include <map>
namespace GRelated {
    class GRGLContext;
    class GRGLDepthTestStrategy {
    public:
        GRGLDepthTestStrategy(GRGLContext& context)
            : m_context(context)
        {

        }
        ~GRGLDepthTestStrategy() = default;

        void setDepthTest(bool);

        bool depthTest() const
        {
            return m_depthTest;
        }

        void setDetphTestFunc(GlobalState);

        GlobalState detphTestFun() const
        {
            return m_depthFunc;
        }

        void apply();
    private:
        GRGLContext& m_context;
        bool m_depthTest = false;
        bool m_applied = false;
        GlobalState m_depthFunc = GlobalState::kALWAYS;
    };

    class GRGLStencilTestStrategy {
    public:
        // TODO
    };

    class GRGLBlendStrategy {
    public:
        GRGLBlendStrategy(GRGLContext& context)
            :m_context(context)
        {

        }
        ~GRGLBlendStrategy() = default;

        void setDrawBufferBlend(GRGL_uint index, bool);
        bool drawBufferBlend(GRGL_uint index) const;

        void setBlendEquation(GlobalState rgb, GlobalState alpha);

        void setBlendFunc(GlobalState srcRgb, GlobalState dstRgb,
            GlobalState srcAlpha, GlobalState dstAlpha);

        void apply();

    private:
        GRGLContext& m_context;
        std::map<GRGL_uint, bool> m_blend;
        bool m_applied = false;
        GlobalState m_rgbEq = GlobalState::kFUNC_ADD;
        GlobalState m_alphaEq = GlobalState::kFUNC_ADD;
        GlobalState m_srcRgb = GlobalState::kONE_MINUS_SRC_ALPHA;
        GlobalState m_dstRgb = GlobalState::kSRC_ALPHA;
        GlobalState m_srcAlpha = GlobalState::kONE;
        GlobalState m_dstAlpha = GlobalState::kZERO;
    };
}

#endif // !_GRGL_PER_SAMPLE_TEST_H

