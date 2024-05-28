

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "sphere.h"
#include "Sphere2.h"
#include "cubes.h"
#include "directLight.h"
#include "SpotLight.h"
#include "cube.h"
#include "maindoor.h"
#include "stb_image.h"
#include "curve.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
//void bed(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
unsigned int loadTextures(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
void addTree(glm::vec2 pos, int height);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1000;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;


//Player
bool isPlayer = true;
glm::vec3 playerFacePos;
glm::vec3 playerFaceDir;
glm::vec3 Up;
glm::vec3 Right;
glm::vec3 WorldUp;
glm::vec3 Target;
float Yaw;
float Pitch;
float Roll;
int boxLeft[3] = { 10,10,10 };
int currentBoxType = 0;

//door 
bool doors = false;

//Shooting..!
bool trigger = false;

//Background Color
float t = 0.0f;
float timeForColor;
bool waitForDayNight = false;
bool setTime = false;

//Cube Calculation..!(1000*1000*1000 grid)
vector <vector < glm::vec3 >> SpawnPos(4, vector<glm::vec3>());
vector <glm::vec3> SpawnDir;

vector <vector<vector<bool>>> isCube(1000, vector<vector<bool> >(1000, vector<bool>(1000, false)));
vector <vector<vector<int>>> isCubeType(1000, vector<vector<int> >(1000, vector<int>(1000, -1)));



// camera
Camera camera(glm::vec3(0.0f, 2.0f, 6.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);


// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(1.5f,  1.5f,  3.0f),
    glm::vec3(-1.5f,  1.5f,  0.0f),
    glm::vec3(-1.5f,  1.5f,  3.0f),
    glm::vec3(-14.0f,  1.50f,  8.0f),
    glm::vec3(12.0f,  1.0f,  11.0f),
    glm::vec3(-12.5f, 1.0f, -13.0f),
    glm::vec3(8.0f,  1.5f,  -14.0f)
};


glm::vec3 lightPositions[] = {
        glm::vec3(1.50f,  2.50f,  0.0f),
        glm::vec3(1.0f,  1.0f,  1.0f)
};

glm::vec3 lightDirections[] = {
    glm::vec3(0.0f, -1.0f, 0.0f)
};

DirectionalLight dLight = DirectionalLight(-0.2f, -1.0f, -0.3f);

SpotLight spotLight(lightPositions[1], lightDirections[0], 2.5f, 15.5f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1);


PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);
PointLight pointlight5(

    pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    5       // light number
);
PointLight pointlight6(

    pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    6       // light number
);
PointLight pointlight7(

    pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    7       // light number
);
PointLight pointlight8(

    pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    8       // light number
);




// light settings
bool pointLightOn = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool directionLightOn = true;
bool SpotlightOn = true;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
float player_foot;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    Shader lightingShader2("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShadingTest.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    GLfloat points[] = {
0.0100, 0.1250, 5.1000,
-0.0850, 0.0650, 5.1000,
-0.2300, 0.0550, 5.1000,
-0.3600, 0.0850, 5.1000,
-0.4800, 0.2250, 5.1000,
-0.5050, 0.4700, 5.1000,
-0.3050, 0.6350, 5.1000,
-0.2050, 0.6700, 5.1000,
-0.3800, 0.7450, 5.1000,
-0.5550, 0.8900, 5.1000,
-0.5000, 1.0900, 5.1000,
-0.3650, 1.1800, 5.1000,
-0.2000, 1.2050, 5.1000,
-0.3000, 1.3850, 5.1000,
-0.3000, 1.4500, 5.1000,
-0.2150, 1.5300, 5.1000,
-0.1050, 1.5700, 5.1000,
-0.0700, 1.5800, 5.1000,
-0.0100, 1.5950, 5.1000,
    };

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
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

    unsigned int cubeVAO, cubeVBO, cubeEBO;
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

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //line
    float lineFromCamera[6];
    //lien VBO,VAO

    unsigned int lineVBO, lineVAO;
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);

    glBindVertexArray(lineVAO);

    // Bind VBO and copy vertex data
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineFromCamera), lineFromCamera, GL_STATIC_DRAW);

    // Specify the vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);




    //objects
    Sphere sphere = Sphere(1.0f, 36, 18, glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f);


    
    string diffuseMapPathg = "grass.png";
    string specularMapPathg = "container2_specular.png";
    unsigned int diffMapg = loadTextures(diffuseMapPathg.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapg = loadTextures(specularMapPathg.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube grass = Cube(diffMapg, specMapg, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathl = "log.png";
    string specularMapPathl = "container2_specular.png";
    unsigned int diffMapl = loadTextures(diffuseMapPathl.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapl = loadTextures(specularMapPathl.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube log = Cube(diffMapl, specMapl, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //leaf
    string diffuseMapPathle = "leaf3.png";
    string specularMapPathle = "container2_specular.png";
    unsigned int diffMaple = loadTextures(diffuseMapPathle.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMaple = loadTextures(specularMapPathle.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube leaf = Cube(diffMaple, specMaple, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);



    //sky wall
    string diffuseMapPaths = "sky.png";
    string specularMapPaths = "container2_specular.png";
    unsigned int diffMaps = loadTextures(diffuseMapPaths.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMaps = loadTextures(specularMapPaths.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube sky = Cube(diffMaps, specMaps, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //boundary wall
    string diffuseMapPathw = "brick2.jpg";
    string specularMapPathw = "container2_specular.png";
    unsigned int diffMapw = loadTextures(diffuseMapPathw.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapw = loadTextures(specularMapPathw.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube wall = Cube(diffMapw, specMapw, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //door
    string diffuseMapPathd = "door.png";
    string specularMapPathd = "container2_specular.png";
    unsigned int diffMapd = loadTextures(diffuseMapPathd.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapd = loadTextures(specularMapPathd.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube door = Cube(diffMapd, specMapd, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    

    string diffuseMapPathsu = "sun4.jpg";
    string specularMapPathsu = "container2_specular.png";
    unsigned int diffMapsu = loadTextures(diffuseMapPathsu.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapsu = loadTextures(specularMapPathsu.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere2 sun = Sphere2(1.0f, 36, 18, glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffMapsu, specMapsu, 0.0f, 0.0f, 1.0f, 1.0f);
    

    string diffuseMapPathde = "shoe.png";
    string specularMapPathde = "container2_specular.png";
    unsigned int diffMapde = loadTextures(diffuseMapPathde.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapde = loadTextures(specularMapPathde.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube shoe = Cube(diffMapde, specMapde, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //tree
    addTree(glm::vec2(5.0, 5.0), 3);
    addTree(glm::vec2(8.0, -5.0), 6);
    addTree(glm::vec2(-5.0, 0.0), 4);
    addTree(glm::vec2(-5.0, -5.0), 5);
    
    
    //Base
    float gridSize = 16;
    for (float i = -gridSize; i < gridSize; i = i + 1.0f)
    {
        for (float j = -gridSize; j < gridSize; j = j + 1.0f)
        {

            isCube[(int)i + 500][0 + 500][(int)j + 500] = true;
            isCube[(int)i + 500][-1 + 500][(int)j + 500] = true;
            isCubeType[(int)i + 500][0 + 500][(int)j + 500] = 1;
            isCubeType[(int)i + 500][-1 + 500][(int)j + 500] = 2;

            SpawnPos[1].push_back(glm::vec3(i, 0.0, j));
            SpawnPos[2].push_back(glm::vec3(i, -1.0, j));


        }
    }
    
    
    double doorOpenTime = 0.0;

    float r = 0.0;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------

        glm::vec4 dawnColor(0.6f, 0.6f, 0.7f, 1.0f);
        glm::vec4 duskColor(0.01f, 0.01f, 0.02f, 1.0f); // Darker blue
        glm::vec4 currentColor;

        //calculation for directional light color and background color...!
        if (!isPlayer)
        {

            float x = sin(glm::radians(t));


            if (t > 89 && t < 90) {
                waitForDayNight = true;
                if (!setTime) {
                    timeForColor = currentFrame;
                    setTime = true;
                }

                if (currentFrame - timeForColor > 15.0) {
                    t++;
                    waitForDayNight = false;
                    setTime = false;
                }

            }

            if (t < 89 && !waitForDayNight)
                t += .05;
            if (t > 90 && !waitForDayNight)
                t += .05;


            if (t > 179 && t < 180) {
                waitForDayNight = true;
                if (!setTime) {
                    timeForColor = currentFrame;
                    setTime = true;
                }

                if (currentFrame - timeForColor > 15.0) {
                    t++;
                    if (t >= 180) {
                        t = 0.0;
                    }
                    waitForDayNight = false;
                    setTime = false;
                }

            }

            currentColor = dawnColor * (1.0f - abs(x)) + duskColor * abs(x);
            glClearColor(currentColor.r, currentColor.g, currentColor.b, currentColor.a);

            ////////
        }
        else
        {
            float x = 0;
            currentColor = dawnColor * (1.0f - abs(x)) + duskColor * abs(x);
            glClearColor(currentColor.r, currentColor.g, currentColor.b, currentColor.a);
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);


        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);
        pointlight5.setUpPointLight(lightingShader);
        // point light 2
        pointlight6.setUpPointLight(lightingShader);
        // point light 3
        pointlight7.setUpPointLight(lightingShader);
        // point light 4
        pointlight8.setUpPointLight(lightingShader);
        // directional light
        dLight.setUpDirectionalLight(lightingShader);

        //spotLight.setUpLight(lightingShader);
        CurveObj co1;
        co1.hollowBezier(points, 18, lightingShader);
        


        lightingShader2.use();
        lightingShader2.setVec3("viewPos", camera.Position);
        spotLight.setUpLight(lightingShader2);
        
        

        
        

       
        //Texture_Sttings

        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);



        
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);

        pointlight5.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight6.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight7.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight8.setUpPointLight(lightingShaderWithTexture);

        spotLight.setUpLight(lightingShaderWithTexture);

       

        // Modelling Transformation
        lightingShader.use();
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, newmodel;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        newmodel = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);

        

       /* glm::mat4 modelForSphere1 = glm::mat4(1.0f);
        modelForSphere1 = glm::translate(model, glm::vec3(-2.0f, 5.2f, -1.0f));
        modelForSphere1 = glm::scale(modelForSphere1, glm::vec3(0.65f, 0.65f, 0.65f));
        sphere.drawSphere(lightingShader, modelForSphere1);*/

        
        lightingShaderWithTexture.use();
        glm::mat4 modelForSphere2 = glm::mat4(1.0f);
        modelForSphere2 = glm::translate(model, glm::vec3(-2.0f, 7.2f, -1.0f));
        modelForSphere2 = glm::scale(modelForSphere2, glm::vec3(0.65f, 0.65f, 0.65f));
        sun.drawSphereWithTexture(lightingShaderWithTexture, modelForSphere2);

        

        glm::mat4 modelMatrixForContainer3 = glm::mat4(1.0f);
        modelMatrixForContainer3 = glm::translate(model, glm::vec3(-16.5f, 0.0f, -16.5f));
        modelMatrixForContainer3 = glm::scale(modelMatrixForContainer3, glm::vec3(32.0f, 12.0f, 0.25f));
        sky.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);

        //boundary

        //left
        glm::mat4 modelMatrixForContainer5 = glm::mat4(1.0f);
        modelMatrixForContainer5 = glm::translate(model, glm::vec3(15.5f, 0.0f, -17.0f));
        modelMatrixForContainer5 = glm::scale(modelMatrixForContainer5, glm::vec3(0.25f, 12.0f, 32.5f));
        sky.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer5);

        /*glm::mat4 modelMatrixForContainer6 = glm::mat4(1.0f);
        modelMatrixForContainer6 = glm::translate(model, glm::vec3(4.75f, 0.0f, -5.0f));
        modelMatrixForContainer6 = glm::scale(modelMatrixForContainer6, glm::vec3(0.25f, 3.0f, 10.0f));
        wall.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer6);*/

        //right
        glm::mat4 modelMatrixForContainer4 = glm::mat4(1.0f);
        modelMatrixForContainer4 = glm::translate(model, glm::vec3(-15.5f, 0.0f, -17.0f));
        modelMatrixForContainer4 = glm::scale(modelMatrixForContainer4, glm::vec3(0.25f, 12.0f, 32.5f));
        sky.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer4);

        /*glm::mat4 modelMatrixForContainer7 = glm::mat4(1.0f);
        modelMatrixForContainer7 = glm::translate(model, glm::vec3(4.75f, 0.0f, -5.0f));
        modelMatrixForContainer7 = glm::scale(modelMatrixForContainer6, glm::vec3(0.25f, 3.0f, 10.0f));
        cube4.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer7);*/

        glm::mat4 modelMatrixForContainers = glm::mat4(1.0f);
        modelMatrixForContainers = glm::translate(model, glm::vec3(1.0f, 1.0f, 1.0f));
        modelMatrixForContainers = glm::scale(modelMatrixForContainers, glm::vec3(1.0f, 1.0f, 1.0f));
        shoe.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainers);

        
        

        //door
        
        /*if (doors)
        {
            r += 0.3;
            if (r > 90.0f) {
                r = 90.0f;

            }
            glm::mat4 modelMatrixForContainerwat = glm::mat4(1.0f);
            glm::mat4 modelMatrixForContainerwar = glm::mat4(1.0f);
            glm::mat4 modelMatrixForContainerwas = glm::mat4(1.0f);


            modelMatrixForContainerwar = glm::rotate(identityMatrix, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
            newmodel = glm::translate(identityMatrix, glm::vec3(-2.5f, 1.0f, 5.0f)) * modelMatrixForContainerwar * glm::translate(identityMatrix, glm::vec3(2.5f, -1.0f, -5.0f));
            mdoor(door, lightingShaderWithTexture, newmodel);

        }
        else
        {
            r -= 0.3;
            if (r < 0.0f) {
                r = 0.0f;

            }
            
            glm::mat4 modelMatrixForContainerwat = glm::mat4(1.0f);
            glm::mat4 modelMatrixForContainerwar = glm::mat4(1.0f);
            glm::mat4 modelMatrixForContainerwas = glm::mat4(1.0f);


            modelMatrixForContainerwar = glm::rotate(identityMatrix, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
            newmodel = glm::translate(identityMatrix, glm::vec3(-2.5f, 1.0f, 5.0f)) * modelMatrixForContainerwar * glm::translate(identityMatrix, glm::vec3(2.5f, -1.0f, -5.0f));
            mdoor(door, lightingShaderWithTexture, newmodel);
        }*/

            
        
        


        int i = 0;
        glm::mat4 modelForCube = glm::mat4(1.0f);
        for (auto& a : SpawnPos[0]) {


            modelForCube = glm::translate(model, glm::vec3(a.x - .5, a.y - .5, a.z - .5));
            // if (trigger)
             //   a += SpawnDir[i] * glm::vec3 (1);
            log.drawCubeWithTexture(lightingShaderWithTexture, modelForCube);
            i++;

        }
        for (auto& a : SpawnPos[1]) {


            modelForCube = glm::translate(model, glm::vec3(a.x - .5, a.y - .5, a.z - .5));
            grass.drawCubeWithTexture(lightingShaderWithTexture, modelForCube);


        }
        for (auto& a : SpawnPos[2]) {


            modelForCube = glm::translate(model, glm::vec3(a.x - .5, a.y - .5, a.z - .5));
            wall.drawCubeWithTexture(lightingShaderWithTexture, modelForCube);


        }
        for (auto& a : SpawnPos[3]) {


            modelForCube = glm::translate(model, glm::vec3(a.x - .5, a.y - .5, a.z - .5));
            leaf.drawCubeWithTexture(lightingShaderWithTexture, modelForCube);


        }


        //line draw..!
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        ourShader.setMat4("model", model);
        ourShader.setVec3("color", glm::vec3(1.0f, 1.0f, 0.0f));

        lineFromCamera[0] = camera.ProcessMouseClick().x;
        lineFromCamera[1] = camera.ProcessMouseClick().y - .02;
        lineFromCamera[2] = camera.ProcessMouseClick().z;
        lineFromCamera[3] = camera.ProcessMouseClick().x;
        lineFromCamera[4] = camera.ProcessMouseClick().y + .02;
        lineFromCamera[5] = camera.ProcessMouseClick().z;

        glBindVertexArray(lineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lineFromCamera), lineFromCamera, GL_STATIC_DRAW);
        glLineWidth(8);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);
        



        //Floor detection..!

        player_foot = camera.Position.y - 1.5;
        if (!isCube[(int)floor(camera.Position.x + .5) + 500]
            [(int)ceil(player_foot - .5) + 500]
            [(int)floor(camera.Position.z + .5) + 500] && isPlayer) {
            camera.Position.y -= .1f;
            //jumpCount = 0;
            ////cout << "no floor" << endl;;
        }
        /*else
        {
            jumpCount = 1;
        }*/
        
        


        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{   
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {

        camera = Camera(glm::vec3(0.0f, 2.0f, 6.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        {
            isPlayer = true;
            camera.Position = playerFacePos;
            camera.Front = playerFaceDir;
            camera.Yaw = -135.0f;
            camera.Roll = 0.0f;
            camera.Pitch = 0.0f;
            camera.WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

        }
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        isPlayer = false;
        playerFacePos = camera.Position;
        playerFaceDir = camera.Front;
        
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (isPlayer) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.ProcessKeyboardPlayer(FORWARD, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.ProcessKeyboardPlayer(BACKWARD, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.ProcessKeyboardPlayer(LEFT, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.ProcessKeyboardPlayer(RIGHT, deltaTime);
        }
    }
    
    if (!isPlayer) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.ProcessKeyboard(FORWARD, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.ProcessKeyboard(LEFT, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.ProcessKeyboard(RIGHT, deltaTime);
        }

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            if (rotateAxis_X) rotateAngle_X -= 0.1;
            else if (rotateAxis_Y) rotateAngle_Y -= 0.1;
            else rotateAngle_Z -= 0.1;
        }
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.001;
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.001;
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.001;
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.001;
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.001;
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.001;
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.001;
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.001;
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.001;
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.001;
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.001;
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.001;

        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        {
            rotateAngle_X += 0.1;
            rotateAxis_X = 1.0;
            rotateAxis_Y = 0.0;
            rotateAxis_Z = 0.0;
        }
        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        {
            rotateAngle_Y += 0.1;
            rotateAxis_X = 0.0;
            rotateAxis_Y = 1.0;
            rotateAxis_Z = 0.0;
        }
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        {
            rotateAngle_Z += 0.1;
            rotateAxis_X = 0.0;
            rotateAxis_Y = 0.0;
            rotateAxis_Z = 1.0;
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            camera.ProcessKeyboard(UP, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            camera.ProcessKeyboard(DOWN, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
            camera.ProcessKeyboard(P_UP, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
            camera.ProcessKeyboard(P_DOWN, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
            camera.ProcessKeyboard(Y_LEFT, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
            camera.ProcessKeyboard(Y_RIGHT, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            camera.ProcessKeyboard(R_LEFT, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            camera.ProcessKeyboard(R_RIGHT, deltaTime);
        }
    }
    

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointlight5.turnOff();
            pointlight6.turnOff();
            pointlight7.turnOff();
            pointlight8.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointlight5.turnOn();
            pointlight6.turnOn();
            pointlight7.turnOn();
            pointlight8.turnOn();
            pointLightOn = !pointLightOn;
        }
    }
   

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        if (specularToggle)
        {

            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            pointlight3.turnSpecularOff();
            pointlight4.turnSpecularOff();
            pointlight5.turnSpecularOff();
            pointlight6.turnSpecularOff();
            pointlight7.turnSpecularOff();
            pointlight8.turnSpecularOff();

            specularToggle = !specularToggle;
        }
        else
        {

            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            pointlight3.turnSpecularOn();
            pointlight4.turnSpecularOn();
            pointlight5.turnSpecularOn();
            pointlight6.turnSpecularOn();
            pointlight7.turnSpecularOn();
            pointlight8.turnSpecularOn();

            
            specularToggle = !specularToggle;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        if (diffuseToggle)
        {

            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            pointlight3.turnDiffuseOff();
            pointlight4.turnDiffuseOff();
            pointlight5.turnDiffuseOff();
            pointlight6.turnDiffuseOff();
            pointlight7.turnDiffuseOff();
            pointlight8.turnDiffuseOff();
            diffuseToggle = !diffuseToggle;
        }
        else
        {

            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            pointlight3.turnDiffuseOn();
            pointlight4.turnDiffuseOn();
            pointlight5.turnDiffuseOn();
            pointlight6.turnDiffuseOn();
            pointlight7.turnDiffuseOn();
            pointlight8.turnDiffuseOn();
            diffuseToggle = !diffuseToggle;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        if (ambientToggle)
        {

            pointlight1.turnAmbientOff();
            pointlight2.turnAmbientOff();
            pointlight3.turnAmbientOff();
            pointlight4.turnAmbientOff();
            pointlight5.turnAmbientOff();
            pointlight6.turnAmbientOff();
            pointlight7.turnAmbientOff();
            pointlight8.turnAmbientOff();
            ambientToggle = !ambientToggle;
        }
        else
        {

            pointlight1.turnAmbientOn();
            pointlight2.turnAmbientOn();
            pointlight3.turnAmbientOn();
            pointlight4.turnAmbientOn();
            pointlight5.turnAmbientOn();
            pointlight6.turnAmbientOn();
            pointlight7.turnAmbientOn();
            pointlight8.turnAmbientOn();
            ambientToggle = !ambientToggle;
        }
        
    }
    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        if (directionLightOn)
        {
            dLight.turnOff();
            directionLightOn = !directionLightOn;
        }
        else
        {
            dLight.turnOn();
            directionLightOn = !directionLightOn;
        }
    }
    if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        if (SpotlightOn)
        {
            spotLight.turnOff();
            SpotlightOn = !SpotlightOn;
        }
        else
        {
            spotLight.turnOn();
            SpotlightOn = !SpotlightOn;
        }
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        if (doors == true) {
            doors = false;
        }
        else {
            doors = true;
        }
    
        
    }
     
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (!isPlayer)
        camera.ProcessMouseScroll(static_cast<float>(yoffset));
    if (isPlayer) {
        currentBoxType++;
        currentBoxType = currentBoxType % 3;
    }
}

void drawLine(Shader& ourShader, glm::mat4 model, float lineFromCamera[]) {}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        //std::cout << "Clicked" << endl;

        int x = (int)floor(camera.ProcessMouseClick().x + .5), y = (int)floor(camera.ProcessMouseClick().y + .5), z = (int)floor(camera.ProcessMouseClick().z + .5);
        //int x1 = (int)floor(player.camera.ProcessMouseClick1().x + .5), y1 = (int)floor(player.camera.ProcessMouseClick1().y + .5), z1 = (int)floor(player.camera.ProcessMouseClick1().z + .5);

        //cout << "In" << x << " " << y << " " << z << ":";
        //cout << x1<< " " << y1 << " " << z1 << endl;

        if (!isCube[x + 500][y + 500][z + 500] && boxLeft[currentBoxType] > 0) {

            isCube[x + 500][y + 500][z + 500] = true;
            isCubeType[x + 500][y + 500][z + 500] = currentBoxType;

            SpawnPos[currentBoxType].push_back(glm::vec3((float)x, (float)y, (float)z));

            // cout <<"From Draw" << SpawnPos[0].size() << " " << SpawnPos[1].size() << " " << SpawnPos[2].size() << endl;
            boxLeft[currentBoxType]--;
        }

        //For  Shooting
        //SpawnPos.push_back(player.camera.ProcessMouseClick());
        //SpawnDir.push_back(player.camera.Front);

    }

    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        //trigger = false;
        //std::cout << "Relaease" << endl;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {

    }

    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        int x = (int)floor(camera.ProcessMouseClick().x + .5), y = (int)floor(camera.ProcessMouseClick().y + .5), z = (int)floor(camera.ProcessMouseClick().z + .5);

        //cout << "Dlt" << x << " " << y << " " << z << endl;

        cout << isCube[x + 500][y + 500][z + 500] << " " << isCubeType[x + 500][y + 500][z + 500] << endl;

        if (isCube[x + 500][y + 500][z + 500] && isCubeType[x + 500][y + 500][z + 500] != -1) {



            isCube[x + 500][y + 500][z + 500] = false;

            int i = isCubeType[x + 500][y + 500][z + 500];
            isCubeType[x + 500][y + 500][z + 500] = -1;

            SpawnPos[i].erase(std::remove(SpawnPos[i].begin(), SpawnPos[i].end(), glm::vec3((float)x, (float)y, (float)z)), SpawnPos[i].end());
            //remove(SpawnPos[i].begin(), SpawnPos[i].end(), glm::vec3((float)x, (float)y, (float)z));

            //cout <<"From dlt" << SpawnPos[0].size() << " " << SpawnPos[1].size() << " " << SpawnPos[2].size() << endl;
            boxLeft[i]++;
        }
    }

}

void addTree(glm::vec2 pos, int height) {

    int i;
    for (i = 1; i <= height; i++) {

        isCube[(int)pos.x + 500][i + 500][(int)pos.y + 500] = true;
        isCubeType[(int)pos.x + 500][i + 500][(int)pos.y + 500] = 0;
        SpawnPos[0].push_back(glm::vec3(pos.x, (float)i, pos.y));



    }



    for (int ii = -3; ii <= 3; ii++) {
        for (int jj = -3; jj <= 3; jj++) {
            if (ii | jj) {

                isCube[(int)ii + (int)pos.x + 500][height + 500][(int)pos.y + jj + 500] = true;
                isCubeType[(int)pos.x + ii + 500][height + 500][(int)pos.y + jj + 500] = 3;

                SpawnPos[3].push_back(glm::vec3(pos.x + (float)ii, (float)height, (float)pos.y + (float)jj));
            }
        }
    }

    for (int ii = -2; ii <= 2; ii++) {
        for (int jj = -2; jj <= 2; jj++) {


            isCube[(int)ii + (int)pos.x + 500][height + 1 + 500][(int)pos.y + jj + 500] = true;
            isCubeType[(int)pos.x + ii + 500][height + 1 + 500][(int)pos.y + jj + 500] = 3;

            SpawnPos[3].push_back(glm::vec3(pos.x + (float)ii, (float)height + 1.0, (float)pos.y + (float)jj));

        }
    }

}

unsigned int loadTextures(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
