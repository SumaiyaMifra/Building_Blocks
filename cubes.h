#pragma once

#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

class Cubes
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    unsigned int cubeVAO, cubeVBO, cubeEBO;

    Cubes(glm::vec3 amb = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 diff = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 spec = glm::vec3(1.0f, 1.0f, 1.0f), float shiny = 32.0f)
    {
        set(amb, diff, spec, shiny);
        buildVertices();
    }

    ~Cubes() {}

    void set(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {

        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }





    void drawCube(Shader& lightingShader, glm::mat4 model) const
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", ambient);
        lightingShader.setVec3("material.diffuse", diffuse);
        lightingShader.setVec3("material.specular", specular);
        lightingShader.setFloat("material.shininess", shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

    }

private:
    float sideLength;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    int verticesStride;

    void buildVertices()
    {

        float cube_vertices[] = {
            // positions      // normals
            -.5f, -.5f, -.5f, 0.0f, 0.0f, -1.0f,
            .5f, -.5f, -.5f, 0.0f, 0.0f, -1.0f,
            .5f, .5f, -.5f, 0.0f, 0.0f, -1.0f,
            -.5f, .5f, -.5f, 0.0f, 0.0f, -1.0f,

            .5f, -.5f, -.5f,   1.0f, 0.0f, 0.0f,
            .5f, .5f, -.5f,    1.0f, 0.0f, 0.0f,
            .5f, -.5f, .5f,   1.0f, 0.0f, 0.0f,
            .5f, .5f, .5f,     1.0f, 0.0f, 0.0f,

            -.5f, -.5f, .5f,  0.0f, 0.0f, 1.0f,
            .5f, -.5f, .5f,  0.0f, 0.0f, 1.0f,
            .5f, .5f, .5f,  0.0f, 0.0f, 1.0f,
            -.5f, .5f, .5f,  0.0f, 0.0f, 1.0f,

            -.5f, -.5f, .5f, -1.0f, 0.0f, 0.0f,
            -.5f, .5f, .5f, -1.0f, 0.0f, 0.0f,
            -.5f, .5f, -.5f, -1.0f, 0.0f, 0.0f,
            -.5f, -.5f, -.5f, -1.0f, 0.0f, 0.0f,

            .5f, .5f, .5f,    0.0f, 1.0f, 0.0f,
            .5f, .5f, -.5f,   0.0f, 1.0f, 0.0f,
            -.5f, .5f, -.5f,  0.0f, 1.0f, 0.0f,
            -.5f, .5f, .5f,   0.0f, 1.0f, 0.0f,

            -.5f, -.5f, -.5f,  0.0f, -1.0f, 0.0f,
            .5f, -.5f, -.5f,   0.0f, -1.0f, 0.0f,
            .5f, -.5f, .5f,    0.0f, -1.0f, 0.0f,
            -.5f, -.5f, .5f,   0.0f, -1.0f, 0.0f
        };
        unsigned int cube_indices[] = {
            0, 3, 2,
            2, 1, 0,

            4, 5, 7,
            7, 6, 4,

            8, 9, 10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            22, 23, 20
        };


        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glGenBuffers(1, &cubeEBO);

        glBindVertexArray(cubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // vertex normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

    }


};

#endif // CUBE_H
