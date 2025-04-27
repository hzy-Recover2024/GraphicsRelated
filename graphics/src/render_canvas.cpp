#include "render_canvas.h"
#include "grgl_context.h"
namespace GRelated {


    void Canvas::prepare(GLFWGLContext& context)
    {
        context.grglSetDefaultFramebuffer(FBOType::kDraw);
        context.clearDepth(1.0f);
        context.clearColorfv(0, { 0.15f, 0.15f, 0.15f, 1.0f });
    }

}

