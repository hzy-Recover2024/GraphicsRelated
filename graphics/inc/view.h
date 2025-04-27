/*管理解析成路径后场景图 */
#ifndef _VIEW_H
#define _VIEW_H
#include "grgl_camera.h"
#include "drawable_node_proxy.h"
#include <set>
#include "instance_stream.h"
#include "grgl_object.h"
class GLFWwindow;
namespace GRelated {
    class DrawableNode;
    class GLFWGLContext;
    class View {
    public:
        static View& getInstance()
        {
            static View ins;
            return ins;
        }

        void updateCamera(GLFWGLContext*);

        void addSceneRoot(std::shared_ptr<DrawableNodeProxy> root)
        {
            m_rootList.push_back(root);
        }

        void buildScene();

        void ensureInDrityList(DrawableNode* pNode)
        {
            m_dirtyList.insert(pNode);
        }

        void dispatchNodeProxy();

        void uploadInstance(GLFWGLContext*);

        InstanceStream& instanceStream()
        {
            if (m_insStream == nullptr)
            {
                m_insStream = std::make_unique<InstanceStream>();
            }
            return *m_insStream;
        }

        // UI callback
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    private:
        View() = default;
        static GRGLCamera& getCamera()
        {
            static GRGLCamera camera;
            return camera;
        }
        static float lastX;
        static float lastY;;
        static bool firstMouse;

        std::vector<std::shared_ptr<DrawableNodeProxy>> m_rootList;
        std::set<DrawableNode*> m_dirtyList;
        std::unique_ptr<InstanceStream> m_insStream;
        std::weak_ptr<GRGLBufferObject> m_ubo;
        // octree

    };
}


#endif _VIEW_H
