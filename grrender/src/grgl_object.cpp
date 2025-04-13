#include "grgl_object.h"
#include "grgl_context.h"
#include "glad/glad.h"
#include "GLFW//glfw3.h"
#include <assert.h>
namespace GRelated {
    namespace {
        GRGL_enum targetTrans(GRGLBufferObject::BufferTarget target)
        {
            switch (target)
            {
            case GRelated::GRGLBufferObject::kVBO:
                return GL_ARRAY_BUFFER;
            case GRelated::GRGLBufferObject::kSSBO:
                return GL_SHADER_STORAGE_BUFFER;
            case GRelated::GRGLBufferObject::kEBO:
                return GL_ELEMENT_ARRAY_BUFFER;
            case GRelated::GRGLBufferObject::kUBO:
                return GL_UNIFORM_BUFFER;
            case GRelated::GRGLBufferObject::kATCBO:
                return GL_ATOMIC_COUNTER_BUFFER;
            case GRelated::GRGLBufferObject::kDIBO:
                return GL_DRAW_INDIRECT_BUFFER;
            default:
                break;
            }
            return target;
        }



    }
    GRGLBufferObject::GRGLBufferObject(GRGLContext& context, BufferTarget target,
        GRGL_sizeiptr size,
        void* data, bool writeAble)
        : GRGLGenericObject(context), m_size(size)
    {
        m_state.set(kWriteAble, writeAble);
        m_bound = targetTrans(target);
        ::glGenBuffers(1, &m_id);
        ::glBindBuffer(GL_ARRAY_BUFFER, m_id);
        // ∑÷≈‰ƒ⁄¥Ê
        GLbitfield flag = 0;
        if (writeAble)
        {
            flag |= GL_MAP_WRITE_BIT;
        }
        ::glBufferStorage(GL_ARRAY_BUFFER, size, data, flag);
        ::glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    GRGLBufferObject::~GRGLBufferObject()
    {
        if (m_bound != 0)
        {
            m_context.detachObject(m_bound, this);
        }
        orphan();
        ::glDeleteBuffers(1, &m_id);
    }

    void GRGLBufferObject::bind()
    {
        if (!m_state.test(kBound))
        {
            m_context.attachObject(m_bound, this);
            ::glBindBuffer(m_bound, m_id);
            m_state.set(kBound, true);
        }
    }

    void GRGLBufferObject::unbind()
    {
        if (m_state.test(kBound))
        {
            ::glBindBuffer(m_bound, 0);
            m_context.detachObject(m_bound, this);
            m_state.set(kBound, false);
        }
    }

    void GRGLBufferObject::bindIndex(GRGL_uint index)
    {
        if (m_bound == GL_SHADER_STORAGE_BUFFER ||
            m_bound == GL_ATOMIC_COUNTER_BUFFER ||
            m_bound == GL_UNIFORM_BUFFER)
        {
            m_context.attachObject(m_bound, this);
            ::glBindBufferBase(m_bound, index, m_id);
            m_state.set(kBound, true);
        }
    }

    void GRGLBufferObject::copyFrom(
        GRGLBufferObject& other, GRGL_intptr readOffset, GRGL_intptr writeOffset, GRGL_sizeiptr size)
    {
        glBindBuffer(GL_COPY_READ_BUFFER, m_id);
        glBindBuffer(GL_COPY_WRITE_BUFFER, other.m_id);
        ::glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER,
            readOffset, writeOffset, size);
        glBindBuffer(GL_COPY_READ_BUFFER, 0);
        glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
    }

    void GRGLBufferObject::orphan()
    {
        ::glInvalidateBufferData(m_id);
    }

    void GRGLBufferObject::getData(GRGL_intptr offset, GRGL_sizeiptr size, void* data)
    {
        if (m_state.test(kMapped))
        {
            return;
        }
        glBindBuffer(m_bound, m_id);
        ::glGetBufferSubData(m_bound, offset, size, data);
        glBindBuffer(m_bound, 0);
    }

    void* GRGLBufferObject::map(GRGL_intptr offset, GRGL_sizeiptr length)
    {
        if (!m_state.test(kBound) || m_state.test(kMapped) || !m_state.test(kWriteAble))
        {
            return nullptr;
        }
        return ::glMapBufferRange(m_bound, offset, length, GL_MAP_WRITE_BIT);
    }

    void GRGLBufferObject::unmap()
    {
        if (!m_state.test(kMapped) | !m_state.test(kBound))
        {
            return;
        }
        ::glUnmapBuffer(m_bound);
    }

    GRGLBufferObjectPersistMapped::GRGLBufferObjectPersistMapped(
        GRGLContext& context, BufferTarget target, GRGL_sizeiptr size,
        void* data)
        : GRGLBufferObject(context, target, size, data, true)
    {
        bind();
        m_data = ::glMapBufferRange(m_bound, 0, size,
            GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    }

    GRGLBufferObjectPersistMapped::~GRGLBufferObjectPersistMapped()
    {
        bind();
        ::glUnmapBuffer(m_bound);
    }

    void* GRGLBufferObjectPersistMapped::map(GRGL_intptr, GRGL_sizeiptr)
    {
        return m_data;
    }

    void GRGLBufferObjectPersistMapped::unmap()
    {

    }

}

