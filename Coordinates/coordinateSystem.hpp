//
//  coordinateSystem.hpp
//  GLFWOpenGL
//
//  Created by Wongeun Cho on 2022/09/03.
//

#ifndef coordinateSystem_hpp
#define coordinateSystem_hpp

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


class CoordinateSystem{
public:
    void execute();

private:
    const GLint width = 800, height = 600;
    
    const char* vertexShaderSource =   "#version 330 core\n"
                                                    "layout(location=0) in vec3 aPos;\n"
                                                    "layout(location=1) in vec3 aColor;\n"
                                                    "layout(location=2) in vec2 aTexCoord;\n"
                                                    "out vec3 outColor;\n"
                                                    "out vec2 TexCoord;\n"
                                                    "uniform mat4 model;\n"
                                                    "uniform mat4 view;\n"
                                                    "uniform mat4 projection;\n"
                                                    "void main()\n"
                                                    "{\n"
                                                    "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
                                                    "   outColor = aColor;\n"
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
                                                    "in vec3 outColor;\n"
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
    
    const int rectangleIndecies[6] = {
        0, 1, 2,    //bottom left, bottom right, top left
        1, 2, 3     //bottom right, top left, top right
    };
    
    unsigned int createTexture(std::string filePath, bool isRGBA);

    void processInput(GLFWwindow *window);
    
    unsigned int RECTANGLE_VAO, RECTANGLE_VBO, RECTANGLE_EBO;
    
    int createVertexShader(const char* vertexShader);
    
    int createFragmentShader(const char* fragmentShader);
    
    int createProgram(const char* vertexShader, const char* fragmentShader);
    
    void createRectangle();
    void activateTexture(unsigned int textureID, int textureUnit);
    void drawRectangle(unsigned int textureID);
    void destroyRectangle();
    
    
    //Transformation
    
    void setMatrix(unsigned int shaderID);
    void doTransform(unsigned int shaderProgramID);
    
    void initMVPMatrix(unsigned int shaderProgramID);
    
};

#endif /* coordinateSystem_hpp */


