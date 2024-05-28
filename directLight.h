


#ifndef dLight_h
#define dLight_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"

class DirectionalLight {
public:
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;


    DirectionalLight(float dirX, float dirY, float dirZ,
        glm::vec3  Ambient = glm::vec3(0.8, 0.8, 0.8),
        glm::vec3 Diffuse = glm::vec3(0.8, 0.8, 0.8),
        float specR = 0.50f, float specG = 0.50f, float specB = 0.50f) {

        direction = glm::vec3(dirX, dirY, dirZ);
        ambient = Ambient;
        diffuse = Diffuse;
        specular = glm::vec3(specR, specG, specB);

    }


    // 0.05f, 0.05f, 0.05f,     // ambient
     //    0.8f, 0.8f, 0.8f,     // diffuse
     //    1.0f, 1.0f, 1.0f,

    void setUpDirectionalLight(Shader& lightingShader)
    {
        lightingShader.use();


        lightingShader.setVec3("directionalLight.direction", direction);
        lightingShader.setVec3("directionalLight.ambient", ambientOn * ambient);
        lightingShader.setVec3("directionalLight.diffuse", diffuseOn * diffuse);
        lightingShader.setVec3("directionalLight.specular", specularOn * specular);


    }

    void turnOff()
    {
        ambientOn = 0.0;
        diffuseOn = 0.0;
        specularOn = 0.0;
    }
    void turnOn()
    {
        ambientOn = 1.0;
        diffuseOn = 1.0;
        specularOn = 1.0;
    }

private:
    float ambientOn = 1.0;
    float diffuseOn = 1.0;
    float specularOn = 1.0;
};

#endif /* dLight_h */
