/*opengl framebuffer object ºÍ renderbuffer object*/

#ifndef _GRGL_FBO_H
#define _GRGL_FBO_H

#include "grgl_object.h"
namespace GRelated {

    enum FBOType {
        kRead,
        kDraw
    };

    enum FBOAttachPoint
    {
        kColor,
        kDepth,
        kStencil,
        kDepthStencil
    };

    class GRGLTextureObject;
    class GRGLRenderbufferBase;
    class GRGLFramebufferObject : public GRGLGenericObject {
    protected:
        GRGLFramebufferObject(GRGLContext& context, FBOType type);
    public:
        static std::shared_ptr<GRGLFramebufferObject> create(GRGLContext& context, FBOType type)
        {
            std::shared_ptr<GRGLFramebufferObject> newPtr(new GRGLFramebufferObject(context, type));
            newPtr->registerContext();
            return newPtr;
        }
        GRGLFramebufferObject(const GRGLFramebufferObject&) = delete;
        GRGLFramebufferObject& operator=(const GRGLFramebufferObject&) = delete;
        ~GRGLFramebufferObject() override;

        ObjectType type() const override
        {
            return ObjectType::kFrameBuffer;
        }

        void bind() override;
        void unbind() override;
        void attachTexture(FBOAttachPoint attachPoint, GRGL_uint index, GRGLTextureObject* pTexture);
        void attachRenderbuffer(FBOAttachPoint attachPoint, GRGL_uint index, GRGLRenderbufferBase* pRbuffer);
        bool isComplete();

        // set drawbuffer or readbuffer
        void setReadBufferSource(GRGL_sizei index);
        void setDrawBufferSource(GRGL_sizei* index, GRGL_sizei count);
        void read() {} // TODO

    private:
        bool m_bUsed = false;
    };

    class GRGLRenderbufferBase : public GRGLGenericObject {
    public:
        using GRGLGenericObject::GRGLGenericObject;
        virtual ~GRGLRenderbufferBase() = default;
    protected:
        bool m_bUsed = false;
    };

    class GRGLRenderbufferObject : public GRGLRenderbufferBase {
    protected:
        GRGLRenderbufferObject(GRGLContext& context, ImageFormat format, GRGL_sizei width,
            GRGL_sizei height);
    public:
        static std::shared_ptr<GRGLRenderbufferObject> create(GRGLContext& context, ImageFormat format, GRGL_sizei width,
            GRGL_sizei height)
        {
            std::shared_ptr<GRGLRenderbufferObject> newPtr(
                new GRGLRenderbufferObject(context, format, width, height));
            newPtr->registerContext();
            return newPtr;
        }

        GRGLRenderbufferObject(const GRGLRenderbufferObject&) = delete;
        GRGLRenderbufferObject& operator=(const GRGLRenderbufferObject&) = delete;

        ~GRGLRenderbufferObject() override;
        
        ObjectType type() const override
        {
            return ObjectType::kRenderBuffer;
        }

        void bind() override;
        void unbind() override;
    };

    class GRGLRenderbufferObjectMultiSample : public GRGLRenderbufferBase {
    protected:
        GRGLRenderbufferObjectMultiSample(GRGLContext& context, GRGL_sizei samples,
            ImageFormat format, GRGL_sizei width,
            GRGL_sizei height);
    public:
        static std::shared_ptr<GRGLRenderbufferObjectMultiSample> create(GRGLContext& context, GRGL_sizei samples,
            ImageFormat format, GRGL_sizei width,
            GRGL_sizei height)
        {
            std::shared_ptr<GRGLRenderbufferObjectMultiSample> newPtr(
                new GRGLRenderbufferObjectMultiSample(
                context, samples, format, width, height));
            newPtr->registerContext();
            return newPtr;
        }
        GRGLRenderbufferObjectMultiSample(const GRGLRenderbufferObjectMultiSample&) = delete;
        GRGLRenderbufferObjectMultiSample& operator=(const GRGLRenderbufferObjectMultiSample&) = delete;
        ~GRGLRenderbufferObjectMultiSample() override;

        ObjectType type() const override
        {
            return ObjectType::kRenderBuffer;
        }

        void bind() override;
        void unbind() override;
    };
}

#endif // !_GRGL_FBO_H

