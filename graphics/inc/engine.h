/*代表整个渲染引擎 */
#ifndef _ENGINE_H
#define  _ENGINE_H
#include <memory>
#include "drawable_node.h"
#include "grgl_model.h"
#include "grgl_context.h"
#include "vertex_stream.h"
#include "render_canvas.h"
#include "render_batch.h"
#include "grgl_per_sample_test.h"
namespace GRelated {
    class Engine {
    public:
        ~Engine(){}
        static Engine& getInstance()
        {
            static Engine ins;
            ::srand(1);
            return ins;
        }
        void draw();
        void initContext();
        void initSecne();
        void uploadVertexData();
    private:
        Engine();
        
        std::vector<GRGLModel> m_models;
        std::vector<std::unique_ptr<DrawableNode>> m_root;
        std::unique_ptr<VertexStream> m_vertexData;
        std::unique_ptr<GLFWGLContext> m_context;
        std::unique_ptr<Canvas> m_canvas;
        std::unique_ptr<TriangleBatch> m_triBatch;
        std::unique_ptr<GRGLDepthTestStrategy> m_depthTest;
        std::unique_ptr<GRGLBlendStrategy> m_opaque;
    };
}


#endif _ENGINE_H
