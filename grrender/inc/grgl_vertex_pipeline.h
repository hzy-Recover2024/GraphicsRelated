/*定义一些shader组成的特定的pipeline*/

#ifndef _GRGL_VERTEX_PIPELINE_H
#define _GRGL_VERTEX_PIPELINE_H
#include "grgl_object.h"
namespace GRelated {
    class GRGLProgram;
    class GRGLContext;
    class GRGLVertexPipeline {
    public:
        static GRGLVertexPipeline& getInstance()
        {
            static GRGLVertexPipeline ins;
            return ins;
        }

        void buildPipeline(GRGLContext&);

        GRGLProgram& meshPhong();

    private:
        GRGLVertexPipeline() = default;
        std::shared_ptr<GRGLProgram> m_meshPhong;
    };
}

#endif // !_GRGL_VERTEX_PIPELINE_H

