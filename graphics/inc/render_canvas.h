/*绘制所需的画布、FBO、texture*/

#ifndef _RENDER_CANVAS_H
#define _RENDER_CANVAS_H
#include "grgl_FBO.h"
namespace GRelated {
    class GLFWGLContext;
    class Canvas {
    public:
        //explicit Canvas(GLFWGLContext& context);
        void prepare(GLFWGLContext& context);
    private:
        // FBO
        // 默认清除的颜色
        // 默认清除的深度
    };
}

#endif // !_RENDER_CANVAS_H

