#include "view.h"
#include <assert.h>
#include "GLFW//glfw3.h"
#include "node_visitor.h"
#include "drawable_node.h"
#include "drawable_node_proxy.h"
#include "transform_node.h"
namespace GRelated {
    float View::lastX = 0.f;
    float View::lastY = 0.f;
    bool View::firstMouse = true;

    void View::buildScene()
    {
        if (m_dirtyList.empty())
        {
            return;
        }

        NodeVisitor visitor(NodeVisitorType::kBuildSceneTree);
        for (auto& pNode : m_dirtyList)
        {
            if (!pNode->dirty())
            {
                continue;
            }
            pNode->accept(visitor);
        }

        m_dirtyList.clear();
    }

    void View::dispatchNodeProxy()
    {
        // octree or other struct
    }

    void View::uploadInstance(GLFWGLContext* pContext)
    {

    }

    void View::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            return;
        }

        if (key == GLFW_KEY_W && action == GLFW_PRESS)
            getCamera().ProcessKeyboard(FORWARD);
        if (key == GLFW_KEY_S && action == GLFW_PRESS)
            getCamera().ProcessKeyboard(BACKWARD);
        if (key == GLFW_KEY_A && action == GLFW_PRESS)
            getCamera().ProcessKeyboard(LEFT);
        if (key == GLFW_KEY_D && action == GLFW_PRESS)
            getCamera().ProcessKeyboard(RIGHT);
        if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
            getCamera() = GRGLCamera();
    }

    void View::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
    {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        getCamera().ProcessMouseMovement(xoffset, yoffset);
    }

    void View::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        getCamera().ProcessMouseScroll(static_cast<float>(yoffset));
    }

}