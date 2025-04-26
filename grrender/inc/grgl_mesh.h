/*mesh代表一个待绘制物体*/
#ifndef MESH_H
#define MESH_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 4

namespace GRelated {

    struct Vertex {
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
    };

    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
    };

    class GRGLMesh {
    public:
        // constructor
        GRGLMesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices)
        {
            this->m_vertices = vertices;
            this->m_indices = indices;
            //this->m_textures = textures;
        }

        const std::vector<Vertex>& vertices() const
        {
            return m_vertices;
        }

        const std::vector<unsigned int>& indices() const
        {
            return m_indices;
        }

        bool isReleased() const
        {
            return m_released;
        }

        void releaseData()
        {
            m_vertices.swap(decltype(m_vertices)());
            m_indices.swap(decltype(m_indices)());
        }

    private:
        // mesh Data
        std::vector<Vertex>       m_vertices;
        std::vector<unsigned int> m_indices;
        std::vector<Texture>      m_textures;
        bool m_released = false;
    };
}
#endif
