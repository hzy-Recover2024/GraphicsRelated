/*opengl generic object*/

#ifndef _GRGL_OBJECT_H
#define _GRGL_OBJECT_H

#include "grgl_type.h"
#include <bitset>
namespace GRelated {
    class GRGLContext;
    class GRGLGenericObject {
    public:
        enum ObjectType
        {
            kBuffer,
            kTexture,
            kQuery,
            kFrameBuffer,
            kRenderBuffer,
            kSampler,
            kVAO
        };

        explicit GRGLGenericObject(GRGLContext& context)
            : m_context(context)
        {

        }

        virtual ~GRGLGenericObject()
        {

        }

        GRGL_uint id() const
        {
            return m_id;
        }

        virtual ObjectType type() const = 0;

        virtual void bind() = 0;
        virtual void unbind() = 0;

    protected:
        GRGLContext& m_context;
        GRGL_uint m_id = 0;
        GRGL_enum m_bound = 0;
    };

    // immutable版
    // 该类对象默认拥有可写的映射标记
    class GRGLBufferObject : public GRGLGenericObject {
    public:
        enum BufferTarget
        {
            kVBO,
            kSSBO,
            kEBO,
            kUBO,
            kATCBO,
            kDIBO
        };

        GRGLBufferObject(GRGLContext& context, BufferTarget target, GRGL_sizeiptr size, void* data, bool writeAble);
        virtual ~GRGLBufferObject();

        ObjectType type() const override
        {
            return kBuffer;
        }

        void bind() override;
        void unbind() override;

        void bindIndex(GRGL_uint index);

        void clear() {} //TODO

        void write() {} //TODO

        void copyFrom(GRGLBufferObject& other,
            GRGL_intptr readOffset, GRGL_intptr writeOffset, GRGL_sizeiptr size);

        void orphan();

        void getData(GRGL_intptr offset, GRGL_sizeiptr size, void* data);

        virtual void* map(GRGL_intptr offset, GRGL_sizeiptr length);
        virtual void unmap();
    private:
        enum State
        {
            kWriteAble = 1,
            kBound,
            kMapped
        };
        GRGL_sizeiptr m_size;
        std::bitset<8> m_state;
    };

    class GRGLBufferObjectPersistMapped : public GRGLBufferObject {
    public:
        GRGLBufferObjectPersistMapped(GRGLContext& context, BufferTarget target, GRGL_sizeiptr size, void* data);
        ~GRGLBufferObjectPersistMapped();

        void issueSync() {} // TODO
        void isSync() {} // TODO

        void* underlayData() const
        {
            return m_data;
        }

        void* map(GRGL_intptr offset, GRGL_sizeiptr length) override;
        void unmap() override;

    private:
        void* m_data;
    };
}

#endif // !_GRGL_OBJECT_H

