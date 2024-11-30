//
// Created by not on 2024/11/30.
//

#ifndef OPENGL_STUDY_MESH_H
#define OPENGL_STUDY_MESH_H

#include <string>
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    aiString path;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    Mesh();

    void Draw(Shader shader);

private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    void stepMesh();
};


#endif //OPENGL_STUDY_MESH_H
