#include "grgl_per_sample_test.h"
#include "grgl_context.h"
namespace GRelated {

    void GRGLDepthTestStrategy::setDepthTest(bool control)
    {
        m_depthTest = control;
        m_applied = false;
    }

    void GRGLDepthTestStrategy::setDetphTestFunc(GlobalState func)
    {
        m_depthFunc = func;
        m_applied = false;
    }

    void GRGLDepthTestStrategy::apply()
    {
        if (m_applied)
        {
            return;
        }
        m_applied = true;
        if (m_depthTest)
        {
            m_context.grglEnable(transGlobalState(GlobalState::kDEPTH_TEST));
        }
        else
        {
            m_context.grglDisable(transGlobalState(GlobalState::kDEPTH_TEST));
            return;
        }
        m_context.grglDepthFunc(transGlobalState(m_depthFunc));
    }

    void GRGLBlendStrategy::setDrawBufferBlend(GRGL_uint index, bool control)
    {
        m_applied = false;
        m_blend[index] = control;
    }

    bool GRGLBlendStrategy::drawBufferBlend(GRGL_uint index) const
    {
        return m_blend.at(index);
    }

    void GRGLBlendStrategy::setBlendEquation(GlobalState rgb, GlobalState alpha)
    {
        m_applied = false;
        m_rgbEq = rgb;
        m_alphaEq = alpha;
    }

    void GRGLBlendStrategy::setBlendFunc(GlobalState srcRgb, GlobalState dstRgb, GlobalState srcAlpha, GlobalState dstAlpha)
    {
        m_applied = false;
        m_srcRgb = srcRgb;
        m_dstRgb = dstRgb;
        m_srcAlpha = srcAlpha;
        m_dstAlpha = dstAlpha;
    }

    void GRGLBlendStrategy::apply()
    {
        if (m_applied)
        {
            return;
        }
        m_applied = true;
        for (const auto& pair : m_blend)
        {
            if (pair.second)
            {
                m_context.grglEnablei(transGlobalState(GlobalState::kBLEND), pair.first);
            }
            else
            {
                m_context.grglDisablei(transGlobalState(GlobalState::kBLEND), pair.first);
            }
        }
        m_context.grglBlendEquationSeparate(transGlobalState(m_rgbEq), transGlobalState(m_alphaEq));
        m_context.grglBlendFuncSeparate(transGlobalState(m_srcRgb), transGlobalState(m_dstRgb),
            transGlobalState(m_srcAlpha), transGlobalState(m_dstAlpha));
    }

}

