/*opengl generic object*/

#ifndef _GRGL_OBJECT_H
#define _GRGL_OBJECT_H

#include "grgl_type.h"
#include <bitset>
#include <memory>
#include <string>
#include <set>
namespace GRelated {
    class GRGLContext;
    class GRGLObject;
    using ObjectPtr = std::shared_ptr<GRGLObject>;
    enum class ObjectType
    {
        kBuffer,
        kTexture,
        kQuery,
        kFrameBuffer,
        kRenderBuffer,
        kSampler,
        kVAO,
        kOther,
        kProgram = kOther,
        kShader,
        kSync
    };

    enum class ObjectTarget {
        kVBO,
        kSSBO,
        kEBO,
        kUBO,
        kATCBO,
        kDIBO
    };

    class GRGLObject : public std::enable_shared_from_this<GRGLObject> {
    public:
        virtual ~GRGLObject() = default;
        GRGLObject(const GRGLObject&) = delete;
        GRGLObject& operator=(const GRGLObject&) = delete;

        void registerContext();
        void unregisterContext();
        virtual ObjectType type() const = 0;
    protected:
        explicit GRGLObject(GRGLContext& context)
            : m_context(context)
        {

        }
        GRGLContext& m_context;
    };

    class GRGLGenericObject : public GRGLObject {
    public:

        explicit GRGLGenericObject(GRGLContext& context)
            : GRGLObject(context)
        {

        }

        ~GRGLGenericObject() override
        {

        }

        GRGL_uint id() const
        {
            return m_id;
        }

        virtual void bind() = 0;
        virtual void unbind() = 0;

    protected:
        GRGL_uint m_id = 0;
        GRGL_enum m_bound = 0;
    };

    // immutable版
    // 该类对象默认拥有可写的映射标记
    class GRGLBufferObject : public GRGLGenericObject {
    protected:
        GRGLBufferObject(GRGLContext& context, ObjectTarget target, GRGL_sizeiptr size, void* data, bool writeAble);
    public:
        static std::shared_ptr<GRGLBufferObject> create(
            GRGLContext& context, ObjectTarget target, GRGL_sizeiptr size, void* data, bool writeAble)
        {
            std::shared_ptr<GRGLBufferObject> newPtr(
                new GRGLBufferObject(context, target, size, data, writeAble));
            newPtr->registerContext();
            return newPtr;
        }

        GRGLBufferObject(const GRGLBufferObject&) = delete;
        GRGLBufferObject& operator=(const GRGLBufferObject&) = delete;
        virtual ~GRGLBufferObject();

        ObjectType type() const override
        {
            return ObjectType::kBuffer;
        }

        void bind() override;
        void unbind() override;

        void bindIndex(GRGL_uint index);

        void setTarget(ObjectTarget target);

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
    protected:
        GRGLBufferObjectPersistMapped(GRGLContext& context, ObjectTarget target, GRGL_sizeiptr size, void* data);
    public:
        static std::shared_ptr<GRGLBufferObjectPersistMapped> create(
            GRGLContext& context, ObjectTarget target, GRGL_sizeiptr size, void* data)
        {
            std::shared_ptr<GRGLBufferObjectPersistMapped> newPtr(
                new GRGLBufferObjectPersistMapped(context, target, size, data));
            newPtr->registerContext();
            return newPtr;
        }

        GRGLBufferObjectPersistMapped(const GRGLBufferObjectPersistMapped&) = delete;
        GRGLBufferObjectPersistMapped& operator=(const GRGLBufferObjectPersistMapped&) = delete;
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

    class GRGLVertexArrayObject : public GRGLGenericObject {
    protected:
        explicit GRGLVertexArrayObject(GRGLContext& context);
    public:
        static std::shared_ptr<GRGLVertexArrayObject> create(GRGLContext& context)
        {
            std::shared_ptr<GRGLVertexArrayObject> newPtr(new GRGLVertexArrayObject(context));
            newPtr->registerContext();
            return newPtr;
        }
        GRGLVertexArrayObject(const GRGLVertexArrayObject&) = delete;
        GRGLVertexArrayObject& operator=(const GRGLVertexArrayObject&) = delete;
        ~GRGLVertexArrayObject();

        ObjectType type() const override
        {
            return ObjectType::kVAO;
        }

        void bind();
        void unbind();

        // 配置顶点属性的格式为浮点数
        void vertexAttriPointer(GRGL_uint index,
            GRGL_int size, TypeEnum type,
            bool normalized, GRGL_sizei stride, GRGL_sizeiptr offset);

        // 配置顶点属性的格式为整型
        void vertexAttriIPointer(GRGL_uint index,
            GRGL_int size, TypeEnum type,
            GRGL_sizei stride, GRGL_sizeiptr offset);

        void enableVertexAttribArray(GRGL_uint index);

        void disableVertexAttribArray(GRGL_uint index);

    };

    enum QueryType
    {
        // 深度测试
        kSamples_passed,
        kAny_samples_passed,
        kTime_elapsed,
        kPrimitives_generated
    };

    class GRGLQueryObject : public GRGLObject {
    protected:
        GRGLQueryObject(GRGLContext& context, QueryType type);
    public:
        static std::shared_ptr<GRGLQueryObject> create(GRGLContext& context, QueryType type)
        {
            std::shared_ptr<GRGLQueryObject> newPtr(new GRGLQueryObject(context, type));
            newPtr->registerContext();
            return newPtr;
        }
        GRGLQueryObject(const GRGLQueryObject&) = delete;
        GRGLQueryObject& operator=(const GRGLQueryObject&) = delete;
        ~GRGLQueryObject() override;

        ObjectType type() const override
        {
            return ObjectType::kQuery;
        }

        void begin();
        void end();
        bool setTarget(QueryType type);
        bool isReady() const;
        GRGL_int64 getResult();
        GRGL_uint64 getResultui();
    private:
        bool m_bQuerying = false;
        GRGL_uint m_id;
        GRGL_enum m_bound;
    };

    class GRGLShader;
    class GRGLProgram : public GRGLObject {
    protected:
        explicit GRGLProgram(GRGLContext& context);
    public:
        static std::shared_ptr<GRGLProgram> create(GRGLContext& context)
        {
            std::shared_ptr<GRGLProgram> newPtr(new GRGLProgram(context));
            newPtr->registerContext();
            return newPtr;
        }
        ObjectType type() const override
        {
            return ObjectType::kProgram;
        }
        GRGLProgram(const GRGLProgram&) = delete;
        GRGLProgram& operator=(const GRGLProgram&) = delete;
        ~GRGLProgram() override;

        void attachShader(std::shared_ptr<GRGLShader> shader);
        bool link();

        void use();

    private:
        GRGL_uint m_id;
        bool m_linkState = false;
        std::set<std::shared_ptr<GRGLShader>> m_shaderCache;
    };

    enum class ShaderType
    {
        kVertexShader,
        kTess_control_shader,
        kTess_evaluation_shader,
        kGeometry_shader,
        kFragment_shader,
        kCompute_shader
    };

    class GRGLShader : public GRGLObject {
    protected:
        GRGLShader(GRGLContext& context, ShaderType type, const std::string& source);
    public:
        static std::shared_ptr<GRGLShader> create(GRGLContext& context, ShaderType type,
            const std::string& source)
        {
            std::shared_ptr<GRGLShader> newPtr(new GRGLShader(context, type, source));
            newPtr->registerContext();
            return newPtr;
        }
        ~GRGLShader() override;
    public:

        ObjectType type() const override
        {
            return ObjectType::kShader;
        }

        GRGL_uint id() const
        {
            return m_id;
        }

        bool compile();

        std::string infolog() const
        {
            return m_infolog;
        }

    private:
        GRGL_uint m_id = 0;
        std::string m_code;
        std::string m_infolog;
        bool m_isCompiled = false;
    };

    class GRGLSync : public GRGLObject {
    protected:
        explicit GRGLSync(GRGLContext& context);
    public:
        static std::shared_ptr<GRGLSync> create(GRGLContext& context)
        {
            std::shared_ptr<GRGLSync> newPtr(new GRGLSync(context));
            newPtr->registerContext();
            return newPtr;
        }

        ObjectType type() const override
        {
            return ObjectType::kSync;
        }

        GRGLSync(const GRGLSync&) = delete;
        GRGLSync& operator=(const GRGLSync&) = delete;
        ~GRGLSync();

        bool isSyncNow() const;
    private:
        GRGL_sync m_id;
    };
}

#endif // !_GRGL_OBJECT_H

