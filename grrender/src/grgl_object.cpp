#include "grgl_object.h"
#include "grgl_context.h"
#include "glad/glad.h"
#include "GLFW//glfw3.h"
#include <assert.h>
namespace GRelated {
    namespace {
        GRGL_enum targetTrans(ObjectTarget target)
        {
            switch (target)
            {
            case ObjectTarget::kVBO:
                return GL_ARRAY_BUFFER;
            case ObjectTarget::kSSBO:
                return GL_SHADER_STORAGE_BUFFER;
            case ObjectTarget::kEBO:
                return GL_ELEMENT_ARRAY_BUFFER;
            case ObjectTarget::kUBO:
                return GL_UNIFORM_BUFFER;
            case ObjectTarget::kATCBO:
                return GL_ATOMIC_COUNTER_BUFFER;
            case ObjectTarget::kDIBO:
                return GL_DRAW_INDIRECT_BUFFER;
            default:
                break;
            }
            return static_cast<GRGL_enum>(target);
        }

        GRGL_enum transFromQueryTarget(QueryType type)
        {
            switch (type)
            {
            case GRelated::kSamples_passed:
                return GL_SAMPLES_PASSED;
            case GRelated::kAny_samples_passed:
                return GL_ANY_SAMPLES_PASSED;
            case GRelated::kTime_elapsed:
                return GL_TIME_ELAPSED;
            case GRelated::kPrimitives_generated:
                return GL_PRIMITIVES_GENERATED;
            default:
                break;
            }
        }

    }

    GRGL_enum transEnum(TypeEnum type)
    {
        switch (type)
        {
        case GRelated::kHALF_FLOAT:
            return GL_HALF_FLOAT;
        case GRelated::kFLOAT:
            return GL_FLOAT;
        case GRelated::kBYTE:
            return GL_BYTE;
        case GRelated::kUNSIGNED_BYTE:
            return GL_UNSIGNED_BYTE;
        case GRelated::kSHORT:
            return GL_SHORT;
        case GRelated::kUNSIGNED_SHORT:
            return GL_UNSIGNED_SHORT;
        case GRelated::kINT:
            return GL_INT;
        case GRelated::kUNSIGNED_INT:
            return GL_UNSIGNED_INT;
        default:
            break;
        }
        return type;
    }


    void GRGLObject::registerContext()
    {
        m_context.record(shared_from_this());
    }

    void GRGLObject::unregisterContext()
    {
        m_context.release(shared_from_this());
    }

    // buffer object
    ////////////////////////////////////////////////////////////////////////////
    GRGLBufferObject::GRGLBufferObject(GRGLContext& context, ObjectTarget target,
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
        if (m_state.test(kBound))
        {
            unbind();
        }
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

    void GRGLBufferObject::setTarget(ObjectTarget target)
    {
        m_bound = targetTrans(target);
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
        GRGLContext& context, ObjectTarget target, GRGL_sizeiptr size,
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

    //VAO
    ////////////////////////////////////////////////////////////////////////////////
    GRGLVertexArrayObject::GRGLVertexArrayObject(GRGLContext& context)
        : GRGLGenericObject(context)
    {
        ::glGenVertexArrays(1, &m_id);
    }

    GRGLVertexArrayObject::~GRGLVertexArrayObject()
    {
        ::glDeleteVertexArrays(1, &m_id);
    }


    void GRGLVertexArrayObject::bind()
    {
        ::glBindVertexArray(m_id);
    }


    void GRGLVertexArrayObject::unbind()
    {
        ::glBindVertexArray(0);
    }

    void GRGLVertexArrayObject::vertexAttriPointer(
        GRGL_uint index, GRGL_int size,
        TypeEnum type, bool normalized, GRGL_sizei stride, GRGL_sizeiptr offset)
    {
        ::glVertexAttribPointer(index, size, transEnum(type), normalized, stride,
            reinterpret_cast<const void*>(offset));
    }

    void GRGLVertexArrayObject::enableVertexAttribArray(GRGL_uint index)
    {
        ::glEnableVertexAttribArray(index);
    }

    void GRGLVertexArrayObject::disableVertexAttribArray(GRGL_uint index)
    {
        ::glDisableVertexAttribArray(index);
    }

    void GRGLVertexArrayObject::vertexAttriIPointer(GRGL_uint index, GRGL_int size, TypeEnum type,
        GRGL_sizei stride, GRGL_sizeiptr offset)
    {
        ::glVertexAttribIPointer(index, size, transEnum(type), stride,
            reinterpret_cast<const void*>(offset));
    }


    GRGLQueryObject::GRGLQueryObject(GRGLContext& context, QueryType type)
        : GRGLObject(context)
    {
        m_bound = transFromQueryTarget(type);
        ::glGenQueries(1, &m_id);
    }

    GRGLQueryObject::~GRGLQueryObject()
    {
        if (m_bQuerying)
        {
            end();
            getResult();
        }
        ::glDeleteQueries(1, &m_id);
    }

    void GRGLQueryObject::begin()
    {
        if (!m_bQuerying)
        {
            ::glBeginQuery(m_bound, m_id);
            m_bQuerying = true;
        }
    }

    void GRGLQueryObject::end()
    {
        if (m_bQuerying)
        {
            ::glEndQuery(m_bound);
            m_bQuerying = false;
        }
    }

    bool GRGLQueryObject::setTarget(QueryType type)
    {
        if (!m_bQuerying)
        {
            m_bound = type;
            return true;
        }
        return false;
    }

    bool GRGLQueryObject::isReady() const
    {
        if (m_bQuerying)
        {
            GRGL_int64 res = 0;
            ::glGetQueryObjecti64v(m_id, GL_QUERY_RESULT_AVAILABLE, &res);
            return res;
        }
        return false;
    }

    GRGL_int64 GRGLQueryObject::getResult()
    {
        if (m_bQuerying)
        {
            GRGL_int64 res = 0;
            ::glGetQueryObjecti64v(m_id, GL_QUERY_RESULT_AVAILABLE, &res);
            return res;
        }
        return 0;
    }

    GRGL_uint64 GRGLQueryObject::getResultui()
    {
        if (m_bQuerying)
        {
            GRGL_uint64 res = 0;
            ::glGetQueryObjectui64v(m_id, GL_QUERY_RESULT_AVAILABLE, &res);
            return res;
        }
        return 0;
    }

    GRGLSync::GRGLSync(GRGLContext& context)
        : GRGLObject(context)
    {
        m_id = reinterpret_cast<GRGL_sync>(::glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0));
    }


    GRGLSync::~GRGLSync()
    {
        ::glDeleteSync(reinterpret_cast<GLsync>(m_id));
    }

    bool GRGLSync::isSyncNow() const
    {
        auto res = ::glClientWaitSync(reinterpret_cast<GLsync>(m_id),
            GL_SYNC_FLUSH_COMMANDS_BIT, 1000);
        return (res == GL_ALREADY_SIGNALED) || (res == GL_ALREADY_SIGNALED);
    }

}

