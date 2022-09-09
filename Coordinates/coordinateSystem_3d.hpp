//
//  coordinateSystem_3d.hpp
//  GLFWOpenGL
//
//  Created by Wongeun Cho on 2022/09/03.
//

#ifndef coordinateSystem_3d_hpp
#define coordinateSystem_3d_hpp

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


class CoordinateSystem3D{
public:
    void execute();

private:
    const GLint width = 800, height = 600;
    
    const char* vertexShaderSource =   "#version 330 core\n"
                                                    "layout(location=0) in vec3 aPos;\n"
                                                    "layout(location=1) in vec2 aTexCoord;\n"
                                                    "out vec2 TexCoord;\n"
                                                    "uniform mat4 model;\n"
                                                    "uniform mat4 view;\n"
                                                    "uniform mat4 projection;\n"
                                                    "void main()\n"
                                                    "{\n"
                                                    "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
                                                    "   TexCoord = aTexCoord;\n"
                                                    "}\0";
                
    
    const char* fragmentShaderSource = "#version 330 core\n"
                                                    "out vec4 FragColor;\n"
                                                    "in vec3 outColor;\n"
                                                    "in vec2 TexCoord;\n"
                                                    "uniform sampler2D ourTexture;\n"
                                                    "void main()\n"
                                                    "{\n"
                                                    "   FragColor = texture(ourTexture, TexCoord) * vec4(outColor, 1.0);\n"
                                                    "}\n";
    
    const char* fragmentShaderWithTwoTextures =     "#version 330 core\n"
                                                    "out vec4 FragColor;\n"
                                                    "in vec2 TexCoord;\n"
                                                    "uniform sampler2D texture1;\n"
                                                    "uniform sampler2D texture2;\n"
                                                    "void main()\n"
                                                    "{\n"
                                                    "   FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord),0.3);\n"
                                                    "}\n";
    
    
    

    const float rectangleVertices[32] = {
        //position              //color                 //uv
        -0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f,      0.0f, 0.0f,     //bottom left
        0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,       1.0f, 0.0f,     //bottom right
        -0.5f, 0.5f, 0.0f,      0.0f, 0.0f, 1.0f,       0.0f, 1.0f,     //top left
        0.5f, 0.5f, 0.0f,      1.0f, 1.0f, 0.0f,        1.0f, 1.0f      //top right
    };
    
    float cubeVertices[180] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
    
    glm::vec3 twoCubes[2] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 0.0f,  0.0f,  0.0f)
    };
    
    unsigned int createTexture(std::string filePath, bool isRGBA);

    void processInput(GLFWwindow *window);
    
    unsigned int CUBE_VAO, CUBE_VBO;
    
    int createVertexShader(const char* vertexShader);
    
    int createFragmentShader(const char* fragmentShader);
    
    int createProgram(const char* vertexShader, const char* fragmentShader);
    
    void createCube();
    void drawCube();
    void destroyCube();
    
    void drawCubes(unsigned int shaderProgram);


    void activateTexture(unsigned int textureID, int textureUnit);    
    
    //Transformation
    
    void setMatrix(unsigned int shaderID);
    void doTransform(unsigned int shaderProgramID);
    
    void initMVPMatrix(unsigned int shaderProgramID);
    void initMVPMatrices(unsigned int shaderProgramID, int cubeIndex);
    
    void initMVPMatricesForRotating(unsigned int shaderProgramID, int cubeIndex);
};

#endif /* coordinateSystem_3d_hpp */

