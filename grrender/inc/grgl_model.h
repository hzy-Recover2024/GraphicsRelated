/* 加载模型并解析成为mesh*/
#ifndef _GRGL_MODEL_H
#define _GRGL_MODEL_H

#include "grgl_mesh.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
namespace GRelated {
    class GRGLModel
    {
    public:
        // model data 
        //std::vector<Texture> m_textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        std::vector <std::shared_ptr<GRGLMesh>> m_meshes;
        std::string m_directory;
        bool m_gammaCorrection;

        // constructor, expects a filepath to a 3D model.
        GRGLModel(std::string const& path, bool gamma = false)
            : m_gammaCorrection(gamma)
        {
            loadModel(path);
        }

    private:
        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        void loadModel(std::string const& path);

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void processNode(aiNode* node, const aiScene* scene);

        std::shared_ptr<GRGLMesh> processMesh(aiMesh* mesh, const aiScene* scene);

        // checks all material textures of a given type and loads the textures if they're not loaded yet.
        // the required info is returned as a Texture struct.
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat,
            aiTextureType type, std::string typeName);
    };

}
#endif
