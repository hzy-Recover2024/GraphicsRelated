/*¶¨Òåvertex specification£¬°üÀ¨VBO¡¢EBO¡¢VAO*/

#ifndef _VERTEX_STREAM_H
#define _VERTEX_STREAM_H
#include "grgl_object.h"
#include <vector>
#include <map>
namespace GRelated {
    class GLFWGLContext;
    class DrawableNode;
    class GRGLVertexArrayObject;
    class VertexStream {
    public:
        void addNode(const DrawableNode* pNode);
        void uploadVertex(GLFWGLContext* pContext);
        GRUINT32 getVertexOffset(const DrawableNode* pNode);
        GRUINT32 getIndexOffset(const DrawableNode* pNode);

        void setupVertexFormat(std::shared_ptr<GRGLVertexArrayObject> vao);
    private:
        struct NodeVertexOffset
        {
            const DrawableNode* dn = nullptr;
            GRUINT32 vertexOffset = 0;
            GRUINT32 indexOffset = 0;
        };
        bool m_needUpade = false;
        std::vector<NodeVertexOffset> m_nodelist;
        std::map<const DrawableNode*, int> m_loactionMap;
        GRUINT32 m_vertexCountProbably = 0;
        GRUINT32 m_indexCountProbably = 0;
        std::weak_ptr<GRGLBufferObject> m_vbo;
        std::weak_ptr<GRGLBufferObject> m_ebo;
    };
    
}

#endif // !_VERTEX_STREAM_H

