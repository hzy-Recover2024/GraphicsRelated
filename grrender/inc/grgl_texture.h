/*opengl texture object ºÍ sampler object*/

#ifndef _GRGL_TEXTURE_H
#define _GRGL_TEXTURE_H

#include "grgl_object.h"
namespace GRelated {
    enum TextureTarget
    {
        kTexture1D,
        kTexture2D,
        kTexture1DArray,
        kTexture2DArray,
        kTexture2DMultiSample
    };

    enum TextureParameter {
        kNearest,
        kLinear,
        miniStart,
        kNearest_mipmap_nearest,
        kLinear_mipmap_nearest,
        kNearest_mipmap_linear,
        kLinear_mipmap_linear,
        miniEnd,
        wrapStart,
        kRepeat,
        kMirrored_repeat,
        kClamp_to_edge,
        kClamp_to_border,
        wrapEnd,
        kDefault
    };

    // immutable storage
    class GRGLTextureObject : public GRGLGenericObject {
    protected:
        GRGLTextureObject(GRGLContext& context, TextureTarget target,
            ImageFormat internalFormat);
    public:
        GRGLTextureObject(const GRGLTextureObject&) = delete;
        GRGLTextureObject& operator=(const GRGLTextureObject&) = delete;
        ~GRGLTextureObject() override;

        ObjectType type() const override
        {
            return ObjectType::kTexture;
        }

        void bind() override;
        void unbind() override;

        bool isMipMap() const
        {
            return m_levels > 1;
        }

        void bindTextureUnit(GRUINT32 index);
        void setParameter(
            TextureParameter wrapS, TextureParameter wrapT, TextureParameter wrapR,
            TextureParameter magFilter, TextureParameter miniFilter);
        void generateMipMap();

        void write() {} //TODO
        void clear() {} // TODO
        void getData() {} // TODO
        void orphan();

        virtual GRGL_sizei width() const = 0;
        virtual GRGL_sizei height() const = 0;
    protected:
        enum TextureAttri
        {
            kBound
        };
        std::bitset<8> m_state;
        GRGL_enum m_internalFormat;
        GRGL_sizei m_levels = 1;
    };

    class GRGLTexture1D : public GRGLTextureObject {
    protected:
        GRGLTexture1D(GRGLContext& context, ImageFormat internalFormat, bool mipMap,
             GRGL_sizei width);
    public:
        static std::shared_ptr<GRGLTexture1D> create(GRGLContext& context, ImageFormat internalFormat, bool mipMap,
            GRGL_sizei width)
        {
            std::shared_ptr<GRGLTexture1D> newPtr(
                new GRGLTexture1D(context, internalFormat, mipMap, width));
            newPtr->registerContext();
            return newPtr;
        }
        GRGLTexture1D(const GRGLTexture1D&) = delete;
        GRGLTexture1D& operator=(const GRGLTexture1D&) = delete;
        ~GRGLTexture1D() {}

        GRGL_sizei width() const override
        {
            return m_width;
        }
        GRGL_sizei height() const override
        {
            return 0;
        }
    private:
        GRGL_sizei m_width = 0;
    };

    class GRGLTexture2D : public GRGLTextureObject {
    protected:
        GRGLTexture2D(GRGLContext& context, ImageFormat internalFormat, bool mipMap,
             GRGL_sizei width, GRGL_sizei height);
    public:
        static std::shared_ptr<GRGLTexture2D> create(GRGLContext& context, ImageFormat internalFormat, bool mipMap,
            GRGL_sizei width, GRGL_sizei height)
        {
            std::shared_ptr<GRGLTexture2D> newPtr(
                new GRGLTexture2D(context, internalFormat, mipMap, width, height));
            newPtr->registerContext();
            return newPtr;
        }
        GRGLTexture2D(const GRGLTexture2D&) = delete;
        GRGLTexture2D& operator=(const GRGLTexture2D&) = delete;
        ~GRGLTexture2D() {}

        GRGL_sizei width() const override
        {
            return m_width;
        }
        GRGL_sizei height() const override
        {
            return m_height;
        }
    private:
        GRGL_sizei m_width = 0;
        GRGL_sizei m_height = 0;
    };

    class GRGLTexture2DMultiSample : public GRGLTextureObject {
    protected:
        GRGLTexture2DMultiSample(GRGLContext& context, ImageFormat internalFormat, GRGL_sizei samples,
            GRGL_sizei width, GRGL_sizei height);
    public:
        static std::shared_ptr<GRGLTexture2DMultiSample> create(GRGLContext& context, ImageFormat internalFormat, GRGL_sizei samples,
            GRGL_sizei width, GRGL_sizei height)
        {
            std::shared_ptr<GRGLTexture2DMultiSample> newPtr(
                new GRGLTexture2DMultiSample(context, internalFormat, samples, width, height));
            newPtr->registerContext();
            return newPtr;
        }
        GRGLTexture2DMultiSample(const GRGLTexture2DMultiSample&) = delete;
        GRGLTexture2DMultiSample& operator&(const GRGLTexture2DMultiSample&) = delete;

        ~GRGLTexture2DMultiSample() {}

        GRGL_sizei width() const override
        {
            return m_width;
        }
        GRGL_sizei height() const override
        {
            return m_height;
        }
    private:
        GRGL_sizei m_width = 0;
        GRGL_sizei m_height = 0;
        GRGL_sizei m_samples = 0;
    };

    class GRGLSamplerObject : public GRGLGenericObject {
        // TODO
    };
}

#endif // !_GRGL_TEXTURE_H

