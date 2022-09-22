//
//  LightCaster.hpp
//  GLFWOpenGL
//
//  Created by Wongeun Cho on 2022/09/18.
//

#ifndef LightCaster_hpp
#define LightCaster_hpp

#include <stdio.h>

#include <iostream>

//GLEW

#define GLEW_STATIC
#include <GL/glew.h>    //should be before GLFW.

//GLFW
#include <GLFW/glfw3.h>

#include "StbiWrapper.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CallbackUtils.hpp"


class LightCaster{
public:
    void execute();

private:
    const GLint width = 800, height = 600;
    
    const char* vertexShaderSource =   "#version 330 core\n"
                                                    "layout(location=0) in vec3 aPos;\n"
                                                    "layout(location=1) in vec3 aNormal;\n"
                                                    "layout(location=2) in vec2 aTexCoords;\n"
                                                    "uniform mat4 model;\n"
                                                    "uniform mat4 view;\n"
                                                    "uniform mat4 projection;\n"
                                                    "out vec3 Normal;\n"
                                                    "out vec3 FragPos;\n"
                                                    "out vec2 TexCoords;\n"
                                                    "void main()\n"
                                                    "{\n"
                                                    "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
                                                    "   FragPos = vec3(model * vec4(aPos, 1.0));\n"
                                                    "   Normal = aNormal;\n"
                                                    "   TexCoords = aTexCoords;\n"
                                                    "}\0";
    
    const char* fragmentShaderSource =  "#version 330 core\n"
                                        "struct Material{\n"
                                        "   sampler2D diffuse;\n"
                                        "   sampler2D specular;\n"
                                        "   float shininess;\n"
                                        "};\n"
                                        "struct Light{\n"
//                                        "   vec3 direction;\n"    //directionLight pos
                                        "   vec3 ambient;\n"
                                        "   vec3 diffuse;\n"
                                        "   vec3 specular;\n"
                                        "                   "
                                        "   float constant;\n"
                                        "   float linear;\n"
                                        "   float quadratic;\n"
                                        "                     "
                                        "   vec3 position;\n"  //spot light position
                                        "   vec3 direction;\n" //spot light direction
                                        "   float cutoff;\n"    //spot light cutoff angle.
                                        "   float outCutoff;\n" //spot light out cutoff angle.
                                        "};\n"
                                        "uniform Material material;\n"
                                        "uniform Light light;\n"
                                        "uniform vec3 lightPos;\n"
                                        "out vec4 FragColor;\n"
                                        "uniform vec3 viewPos;\n"
                                        "in vec3 Normal;\n"
                                        "in vec3 FragPos;\n"
                                        "in vec2 TexCoords;\n"
                                        "void main()\n"
                                        "{\n"
//                                        "   vec3 lightDir = normalize(-light.direction);\n"   //direction Light pos
//                                        "   vec3 lightDir = normalize(lightPos - FragPos);\n"
                                        "   vec3 lightDir = normalize(light.position - FragPos);\n"
                                        "   float theta = dot(lightDir, normalize(-light.direction));\n"
                                        "   float epsilon = light.cutoff - light.outCutoff;\n"
                                        "   float intensity = clamp((theta - light.outCutoff)/epsilon,0.0f, 1.0f);\n"
                                        "   vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;\n"
//                                        "   if(theta>light.cutoff){\n"
//                                        "       float distance = length(lightPos - FragPos);\n"
                                        "       float distance = length(light.position - FragPos);\n"
                                        "       float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance *         distance));\n"
                                        "       vec3 norm = normalize(Normal);\n"
                                        "       float diff = max(dot(norm, lightDir), 0.0);\n"
                                        "       vec3 diffuse = vec3(texture(material.diffuse,TexCoords))*diff*light.diffuse;\n"
                                        "       vec3 viewDir = normalize(viewPos - FragPos);\n"
                                        "       vec3 reflectDir = reflect(-lightDir, norm);\n"
                                        "       float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
                                        "       vec3 specular = vec3(texture(material.specular,TexCoords)) * spec * light.specular;\n"
                                        "       diffuse *= intensity;\n"
                                        "       specular *= intensity;\n"
                                        "       FragColor = vec4(((ambient+diffuse+specular)*attenuation), 1.0);\n"
//                                        "   } else{\n"
//                                        "       FragColor = vec4(ambient, 1.0);\n"
//                                        "   }\n"
                                        "}\n";
                                                    
    const char* lampFragmentShaderSource =  "#version 330 core\n"
                                            "out vec4 FragColor;\n"
                                            "void main()\n"
                                            "{\n"
                                            "   FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
                                            "}\n";

    float cubeVertices[288] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    
    glm::vec3 cubePositions[10] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    const std::string containerImage = "/Users/wongeuncho/Desktop/Dev/image/container.png";
    const std::string containerSpecularImage = "/Users/wongeuncho/Desktop/Dev/image/container_specular.png";

    void processInput(GLFWwindow *window);
    
    unsigned int CUBE_VAO, CUBE_VBO;
    unsigned int LIGHT_VAO;
    
    glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
    
    int createVertexShader(const char* vertexShader);
    int createFragmentShader(const char* fragmentShader);
    
    int createProgram(const char* vertexShader, const char* fragmentShader);
    
    
    void createCube();
    void drawCube();
    void destroyCube();
    
    void assignLightUniform(unsigned int shaderProgramID);
    
    void createLamp();
    void destroyLamp();
    
    void drawLamp();
    
    //Transformation
    void initMVPMatrix(unsigned int shaderProgramID);
    void initLampMatrix(unsigned int shaderProgramID);    //Lamp
    
    void initModelMatrix(unsigned int shaderProgramID, int cubeIndex);
    void initViewMatrix(unsigned int shaderProgramID);
    void initProjectionMatrix(unsigned int shaderProgramID);

    
    //Texture
    unsigned int createTexture(std::string filePath, bool isRGBA);

};

#endif /* LightCaster_hpp */
