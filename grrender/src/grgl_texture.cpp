#include "grgl_texture.h"
#include "grgl_context.h"
#include "glad/glad.h"
#include "GLFW//glfw3.h"
#include <assert.h>
namespace GRelated {

    namespace {
        GRGL_enum transFromTarget(TextureTarget target)
        {
            switch (target)
            {
            case GRelated::kTexture1D:
                return GL_TEXTURE_1D;
            case GRelated::kTexture2D:
                return GL_TEXTURE_2D;
            case GRelated::kTexture1DArray:
                return GL_TEXTURE_1D_ARRAY;
            case GRelated::kTexture2DArray:
                return GL_TEXTURE_2D_ARRAY;
            case GRelated::kTexture2DMultiSample:
                return GL_TEXTURE_2D_MULTISAMPLE;
            default:
                break;
            }
            return target;
        }

        GRGL_sizei calculateMipLevels(GRGL_sizei width, GRGL_sizei height) {
            if (width <= 0 || height <= 0) return 0; // ÎÞÐ§³ß´ç
            GRGL_sizei max_dim = std::max(width, height);
            return static_cast<GRGL_sizei>(std::log2(max_dim)) + 1;
        }

        GRGL_enum transTextureParameter(TextureParameter parameter)
        {
            switch (parameter)
            {
            case GRelated::kNearest:
                return GL_NEAREST;
            case GRelated::kLinear:
                return GL_LINEAR;
            case GRelated::kNearest_mipmap_nearest:
                return GL_NEAREST_MIPMAP_NEAREST;
            case GRelated::kLinear_mipmap_nearest:
                return GL_LINEAR_MIPMAP_NEAREST;
            case GRelated::kNearest_mipmap_linear:
                return GL_NEAREST_MIPMAP_LINEAR;
            case GRelated::kLinear_mipmap_linear:
                return GL_LINEAR_MIPMAP_LINEAR;
            case GRelated::kRepeat:
                return GL_REPEAT;
            case GRelated::kMirrored_repeat:
                return GL_MIRRORED_REPEAT;
            case GRelated::kClamp_to_edge:
                return GL_CLAMP_TO_EDGE;
            case GRelated::kClamp_to_border:
                return GL_CLAMP_TO_BORDER;
            default:
                break;
            }
            return parameter;
        }
    }

    GRGL_enum transImageFormat(ImageFormat format)
    {
        switch (format)
        {
            MAP_FORMAT(ImageFormat::kR8, GL_R8);
            MAP_FORMAT(ImageFormat::kRG8, GL_RG8);
            MAP_FORMAT(ImageFormat::kRGB8, GL_RGB8);
            MAP_FORMAT(ImageFormat::kRGBA8, GL_RGBA8);
            MAP_FORMAT(ImageFormat::kR16, GL_R16);
            MAP_FORMAT(ImageFormat::kRG16, GL_RG16);
            MAP_FORMAT(ImageFormat::kRGB16, GL_RGB16);
            MAP_FORMAT(ImageFormat::kRGBA16, GL_RGBA16);
            MAP_FORMAT(ImageFormat::kR16F, GL_R16F);
            MAP_FORMAT(ImageFormat::kRG16F, GL_RG16F);
            MAP_FORMAT(ImageFormat::kRGB16F, GL_RGB16F);
            MAP_FORMAT(ImageFormat::kRGBA16F, GL_RGBA16F);
            MAP_FORMAT(ImageFormat::kR32F, GL_R32F);
            MAP_FORMAT(ImageFormat::kRG32F, GL_RG32F);
            MAP_FORMAT(ImageFormat::kRGB32F, GL_RGB32F);
            MAP_FORMAT(ImageFormat::kRGBA32F, GL_RGBA32F);
            MAP_FORMAT(ImageFormat::kDepth24, GL_DEPTH_COMPONENT24);
            MAP_FORMAT(ImageFormat::kDepth32, GL_DEPTH_COMPONENT32);
            MAP_FORMAT(ImageFormat::kDepth32F, GL_DEPTH_COMPONENT32F);
            MAP_FORMAT(ImageFormat::kDepth24Stencil8, GL_DEPTH24_STENCIL8);
        default:
            break;
        }
        return static_cast<GRGL_enum>(format);
    }

    GRGLTextureObject::GRGLTextureObject(GRGLContext& context, TextureTarget target,
        ImageFormat internalFormat)
        : GRGLGenericObject(context)
    {
        ::glGenTextures(1, &m_id);
        m_bound = transFromTarget(target);
        m_internalFormat = transImageFormat(internalFormat);
    }

    GRGLTextureObject::~GRGLTextureObject()
    {
        if (m_state.test(kBound))
        {
            unbind();
        }
        ::glDeleteTextures(1, &m_id);
    }

    void GRGLTextureObject::bind()
    {
        if (!m_state.test(kBound))
        {
            ::glBindTexture(m_bound, m_id);
            m_state.set(kBound, true);
            m_context.attachObject(m_bound, this);
        }
    }

    void GRGLTextureObject::unbind()
    {
        if (m_state.test(kBound))
        {
            m_state.set(kBound, false);
            m_context.detachObject(m_bound, this);
        }
    }

    void GRGLTextureObject::bindTextureUnit(GRUINT32 index)
    {
        ::glActiveTexture(GL_TEXTURE0 + index);
        bind();
    }


    void GRGLTextureObject::setParameter(TextureParameter wrapS,
        TextureParameter wrapT, TextureParameter wrapR,
        TextureParameter magFilter, TextureParameter miniFilter)
    {
        if (wrapS > wrapStart && wrapS < wrapEnd)
        {
            ::glTexParameteri(m_bound, GL_TEXTURE_WRAP_S, transTextureParameter(wrapS));
        }
        if (wrapT > wrapStart && wrapT < wrapEnd)
        {
            ::glTexParameteri(m_bound, GL_TEXTURE_WRAP_T, transTextureParameter(wrapT));
        }
        if (wrapR > wrapStart && wrapR < wrapEnd)
        {
            ::glTexParameteri(m_bound, GL_TEXTURE_WRAP_R, transTextureParameter(wrapR));
        }
        if (magFilter < miniStart)
        {
            ::glTexParameteri(m_bound, GL_TEXTURE_MAG_FILTER, transTextureParameter(magFilter));
        }
        if (miniFilter > miniStart && miniFilter < miniEnd)
        {
            ::glTexParameteri(m_bound, GL_TEXTURE_MIN_FILTER, transTextureParameter(miniFilter));
        }
    }

    void GRGLTextureObject::generateMipMap()
    {
        if (m_levels > 1)
        {
            ::glGenerateMipmap(m_bound);
        }
    }

    void GRGLTextureObject::orphan()
    {
        for (GRGL_sizei i = 0; i < m_levels; ++i)
        {
            bind();
            ::glInvalidateTexImage(m_bound, i);
            unbind();
        }
    }

    GRGLTexture1D::GRGLTexture1D(GRGLContext& context,
        ImageFormat internalFormat,
        bool mipMap, GRGL_sizei width)
        : GRGLTextureObject(context, kTexture1D, internalFormat),
        m_width(width)
    {
        m_levels = mipMap ? calculateMipLevels(width, width) : 1;
        ::glBindTexture(m_bound, m_id);
        ::glTexStorage1D(m_bound, m_levels, m_internalFormat, width);
        ::glBindTexture(m_bound, 0);
    }

    GRGLTexture2D::GRGLTexture2D(GRGLContext& context,
        ImageFormat internalFormat,
        bool mipMap, GRGL_sizei width, GRGL_sizei height)
        : GRGLTextureObject(context, kTexture2D, internalFormat),
        m_width(width), m_height(height)
    {
        m_levels = mipMap ? calculateMipLevels(width, width) : 1;
        ::glBindTexture(m_bound, m_id);
        ::glTexStorage2D(m_bound, m_levels, m_internalFormat, width, height);
        ::glBindTexture(m_bound, 0);
    }

    GRGLTexture2DMultiSample::GRGLTexture2DMultiSample(GRGLContext& context,
        ImageFormat internalFormat, GRGL_sizei samples, GRGL_sizei width, GRGL_sizei height)
        : GRGLTextureObject(context, kTexture2DMultiSample, internalFormat),
        m_width(width), m_height(height), m_samples(samples)
    {
        ::glBindTexture(m_bound, m_id);
        ::glTextureStorage2DMultisample(m_bound, samples, m_internalFormat, width, height, true);
        ::glBindTexture(m_bound, 0);
    }
}

