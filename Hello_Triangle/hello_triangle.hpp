//
//  hello_triangle.hpp
//  GLFW OpenGL
//
//  Created by Wongeun Cho on 2022/08/04.
//

#ifndef hello_triangle_hpp
#define hello_triangle_hpp

#include <stdio.h>

#include <iostream>

//GLEW

#define GLEW_STATIC
#include <GL/glew.h>    //should be before GLFW.

//GLFW
#include <GLFW/glfw3.h>

class HelloTriangle{
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
                                        "void main()\n"
                                        "{\n"
                                        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                        "}\n";
    
    const float vertices[9] = {
        -0.5f, -0.5f, 0.0f, //bottom left
        0.5f, -0.5f, 0.0f,  //bottom right
        0.0f, 0.5f, 0.0f    //top center
    };
    
    const float triangleAnotherVertices[9]={
        0.5f, -0.5f, 0.0f,
        1.0f, -0.5f, 0.0f,
        0.75f, 0.5f, 0.0f
    };
    
    
    const float leftTriangleVertices[9]={
        -1.0f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.75f, 0.5f, 0.0f
    };
    
    const float rectangleVertices[12] = {
        -0.5f, -0.5f, 0.0f, //bottom left
        0.5f, -0.5f, 0.0f,  //bottom right
        -0.5f, 0.5f, 0.0f,  //top left
        0.5f, 0.5f, 0.0f    //top right
    };
    
    const int rectangleIndecies[6] = {
        0, 1, 2,    //bottom left, bottom right, top left
        1, 2, 3     //bottom right, top left, top right
    };

    void processInput(GLFWwindow *window);
    
    unsigned int TRIANGLE_VAO[3];
    unsigned int TRIANGLE_VBO[3];
    unsigned int RECTANGLE_VAO, RECTANGLE_VBO, RECTANGLE_EBO;

    
    void createTriangle(const float* vertices, int size, int indexOfTriangle);
    void drawTriangle(int indexOfTriangle);
    void destroyTriangle(int indexOfTriangle);
    
    void createRectangle();
    void drawRectangle();
    void destroyRectangle();
    

};

#endif /* hello_triangle_hpp */
