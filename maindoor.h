#pragma once
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "cube.h"

#include "stb_image.h"



void mdoor(Cube& gate, Shader& lightingShader, glm::mat4 als)
{


    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.5f, 1.0f, 5.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 1.0f, 0.2f));
    model = als * translateMatrix * scaleMatrix;
    gate.drawCubeWithTexture(lightingShader, model);


}