#include "grgl_FBO.h"
#include "grgl_context.h"
#include "grgl_texture.h"
#include "glad/glad.h"
#include "GLFW//glfw3.h"
#include <assert.h>
#include <vector>
namespace GRelated {
    namespace {
        GRGL_enum transFromAttachPoint(FBOAttachPoint at, GRGL_uint i)
        {
            switch (at)
            {
            case GRelated::kColor:
                return GL_COLOR_ATTACHMENT0 + i;
            case GRelated::kDepth:
                return GL_DEPTH_ATTACHMENT;
            case GRelated::kStencil:
                return GL_STENCIL_ATTACHMENT;
            case GRelated::kDepthStencil:
                return GL_DEPTH_STENCIL_ATTACHMENT;
            default:
                break;
            }
        }





    }

    GRGLFramebufferObject::GRGLFramebufferObject(GRGLContext& context, FBOType type)
        : GRGLGenericObject(context)
    {
        ::glGenFramebuffers(1, &m_id);
        if (type == kRead)
        {
            m_bound = GL_READ_FRAMEBUFFER;
        }
        else
        {
            m_bound = GL_DRAW_FRAMEBUFFER;
        }
    }

    GRGLFramebufferObject::~GRGLFramebufferObject()
    {
        if (m_bUsed)
        {
            unbind();
        }
        ::glDeleteFramebuffers(1, &m_id);
    }

    void GRGLFramebufferObject::bind()
    {
        if (!m_bUsed)
        {
            ::glBindFramebuffer(m_bound, m_id);
            m_bUsed = true;
            m_context.attachObject(m_bound, this);
        }
    }

    void GRGLFramebufferObject::unbind()
    {
        if (m_bUsed)
        {
            m_bUsed = false;
            m_context.detachObject(m_bound, this);
        }
    }

    void GRGLFramebufferObject::attachTexture(FBOAttachPoint attachPoint, GRGL_uint index,
        GRGLTextureObject* pTexture)
    {
        ::glFramebufferTexture(m_bound, transFromAttachPoint(attachPoint, index),
            pTexture->id(), 0);
    }

    void GRGLFramebufferObject::attachRenderbuffer(FBOAttachPoint attachPoint, GRGL_uint index, GRGLRenderbufferBase* pRbuffer)
    {
        ::glFramebufferRenderbuffer(m_bound, transFromAttachPoint(attachPoint, index), GL_RENDERBUFFER,
            pRbuffer->id());
    }

    bool GRGLFramebufferObject::isComplete()
    {
        auto res = ::glCheckFramebufferStatus(m_bound);

        return res == GL_FRAMEBUFFER_COMPLETE;
    }

    void GRGLFramebufferObject::setReadBufferSource(GRGL_sizei index)
    {
        if (m_bound == GL_READ_FRAMEBUFFER)
        {
            ::glReadBuffer(GL_COLOR_ATTACHMENT0 + index);
        }
    }

    void GRGLFramebufferObject::setDrawBufferSource(GRGL_sizei* index, GRGL_sizei count)
    {
        if (m_bound == GL_DRAW_FRAMEBUFFER)
        {
            std::vector<GRGL_enum> buffers(count);
            for (GRGL_sizei i = 0; i < count; ++i)
            {
                buffers[i] = GL_COLOR_ATTACHMENT0 + index[i];
            }
            ::glDrawBuffers(count, buffers.data());
        }
    }

    GRGLRenderbufferObject::GRGLRenderbufferObject(GRGLContext& context,
        ImageFormat format, GRGL_sizei width, GRGL_sizei height)
        : GRGLRenderbufferBase(context)
    {
        ::glGenRenderbuffers(1, &m_id);
        m_bound = GL_RENDERBUFFER;
        ::glBindRenderbuffer(m_bound, m_id);
        ::glRenderbufferStorage(m_bound, transImageFormat(format), width, height);
        ::glBindRenderbuffer(m_bound, 0);
    }

    GRGLRenderbufferObject::~GRGLRenderbufferObject()
    {
        if (m_bUsed)
        {
            unbind();
        }
        ::glDeleteRenderbuffers(1, &m_id);
    }

    void GRGLRenderbufferObject::bind()
    {
        if (!m_bUsed)
        {
            ::glBindRenderbuffer(m_bound, m_id);
            m_bUsed = true;
            m_context.attachObject(m_bound, this);
        }
    }

    void GRGLRenderbufferObject::unbind()
    {
        if (m_bound)
        {
            m_bUsed = false;
            m_context.detachObject(m_bound, this);
        }
    }

    GRGLRenderbufferObjectMultiSample::GRGLRenderbufferObjectMultiSample(
        GRGLContext& context, GRGL_sizei samples, ImageFormat format,
        GRGL_sizei width, GRGL_sizei height)
        : GRGLRenderbufferBase(context)
    {
        ::glGenRenderbuffers(1, &m_id);
        m_bound = GL_RENDERBUFFER;
        ::glBindRenderbuffer(m_bound, m_id);
        ::glRenderbufferStorageMultisample(m_bound, samples, transImageFormat(format),
            width, height);
    }

    GRGLRenderbufferObjectMultiSample::~GRGLRenderbufferObjectMultiSample()
    {
        if (m_bUsed)
        {
            unbind();
        }
        ::glDeleteRenderbuffers(1, &m_id);
    }

    void GRGLRenderbufferObjectMultiSample::bind()
    {
        if (!m_bUsed)
        {
            ::glBindRenderbuffer(m_bound, m_id);
            m_bUsed = true;
            m_context.attachObject(m_bound, this);
        }
    }

    void GRGLRenderbufferObjectMultiSample::unbind()
    {
        if (m_bound)
        {
            m_bUsed = false;
            m_context.detachObject(m_bound, this);
        }
    }

}

