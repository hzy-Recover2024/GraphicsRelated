/*代表整个渲染引擎 */
#ifndef _ENGINE_H
#define  _ENGINE_H
#include <memory>
#include "drawable_node.h"
#include "grgl_model.h"
#include "grgl_context.h"
namespace GRelated {
    class Engine {
    public:
        ~Engine(){}
        static Engine& getInstance()
        {
            static Engine ins;
            return ins;
        }
        void draw();
        void initContext();
        void initSecne();
    private:
        Engine() = default;
        
        std::vector<GRGLModel> m_models;
        std::unique_ptr<DrawableNode> m_root;
        std::unique_ptr<GLFWGLContext> m_context;
    };
}


#endif _ENGINE_H
