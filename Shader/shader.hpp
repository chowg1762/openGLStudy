//
//  shader.hpp
//  GLFWOpenGL
//
//  Created by Wongeun Cho on 2022/08/07.
//

#ifndef shader_hpp
#define shader_hpp

#include <stdio.h>


#include <stdio.h>
#include <iostream>

//GLEW

#define GLEW_STATIC
#include <GL/glew.h>    //should be before GLFW.

//GLFW
#include <GLFW/glfw3.h>

#include <cmath>

class Shader{
public:
    void execute();

private:
    const GLint width = 800, height = 600;
    
    const char* vertexShaderSource =    "#version 330 core\n"
                                        "layout(location=0) in vec3 aPos;\n"
                                        "void main()\n"
                                        "{\n"
                                        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                        "}\0";
    
    const char* fragmentShaderSource=   "#version 330 core\n"
                                        "out vec4 FragColor;\n"
                                        "uniform vec3 outColor;\n"
                                        "void main()\n"
                                        "{\n"
                                        "   FragColor = vec4(outColor.xyz, 1.0f);\n"
                                        "}\n";
    
    const char* rasterizationVertexShaderSource =   "#version 330 core\n"
                                                    "layout(location=0) in vec3 aPos;\n"
                                                    "layout(location=1) in vec3 aColor;\n"
                                                    "out vec3 outColor;\n"
                                                    "void main()\n"
                                                    "{\n"
                                                    "   gl_Position = vec4(aPos, 1.0);\n"
                                                    "   outColor = aColor;\n"
                                                    "}\0";
                
    
    const char* rasterizationFragmentShaderSource = "#version 330 core\n"
                                                    "out vec4 FragColor;\n"
                                                    "in vec3 outColor;\n"
                                                    "void main()\n"
                                                    "{\n"
                                                    "   FragColor = vec4(outColor, 1.0f);\n"
                                                    "}\n";
    
    const float vertices[9] = {
        -0.5f, -0.5f, 0.0f, //bottom left
        0.5f, -0.5f, 0.0f,  //bottom right
        0.0f, 0.5f, 0.0f    //top center
    };
    
    const float verticesWithColors[18] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,//bottom left
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, //bottom right
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f //top center
    };
    
    
    unsigned int VAO, VBO;
    
    int createVertexShader(const char* vertexShader);
    
    int createFragmentShader(const char* fragmentShader);
    
    int createProgram(const char* vertexShader, const char* fragmentShader);
    
    void createTriangle();
    void createTriangleColorFromVertices();
    void drawTriangle();
    void destroyTriangle();

    void processInput(GLFWwindow *window);
};

#endif /* shader_hpp */
