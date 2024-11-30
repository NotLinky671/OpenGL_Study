//
// Created by not on 2024/11/30.
//

#ifndef OPENGL_STUDY_MODEL_H
#define OPENGL_STUDY_MODEL_H

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

unsigned int TextureFromFile(const char *path, const std::string& directory);

class Model {
public:
    Model(std::string const path);
    void Draw(Shader shader);

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;

    void loadModel(std::string const path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};


#endif //OPENGL_STUDY_MODEL_H
