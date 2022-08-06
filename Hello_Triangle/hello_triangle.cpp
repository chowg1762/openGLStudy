//
//  hello_triangle.cpp
//  GLFW OpenGL
//
//  Created by Wongeun Cho on 2022/08/04.
//

#include "hello_triangle.hpp"

void HelloTriangle::processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void HelloTriangle::createTriangle(const float* vertices, int sizeOfVertices, int indexOfTriangle){
    std::cout<<"[HelloTriangle::createTriangle] address of vertices "<<vertices<<", size of vert : "<<sizeOfVertices<<std::endl;
    /*Triangle*/
    glGenVertexArrays(1, &TRIANGLE_VAO[indexOfTriangle]);
    glGenBuffers(1, &TRIANGLE_VBO[indexOfTriangle]);
    
    glBindVertexArray(TRIANGLE_VAO[indexOfTriangle]);
    
    glBindBuffer(GL_ARRAY_BUFFER, TRIANGLE_VBO[indexOfTriangle]);
    glBufferData(GL_ARRAY_BUFFER, sizeOfVertices, vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);   //shader 와 연결?
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind
    glBindVertexArray(0);
}

void HelloTriangle::drawTriangle(int indexOfTriangle){
    glBindVertexArray(TRIANGLE_VAO[indexOfTriangle]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void HelloTriangle::destroyTriangle(int indexOfTriangle){
    glDeleteVertexArrays(1, &TRIANGLE_VAO[indexOfTriangle]);
    glDeleteBuffers(1, &TRIANGLE_VBO[indexOfTriangle]);
}

void HelloTriangle::createRectangle(){
    unsigned int RECTANGLE_VBO, RECTANGLE_EBO;
    /*Rectangle*/
    glGenVertexArrays(1, &RECTANGLE_VAO);
    glGenBuffers(1, &RECTANGLE_VBO);
    glGenBuffers(1, &RECTANGLE_EBO);
    
    glBindVertexArray(RECTANGLE_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, RECTANGLE_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RECTANGLE_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndecies), rectangleIndecies, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind
    glBindVertexArray(0);

}

void HelloTriangle::drawRectangle(){
    glBindVertexArray(RECTANGLE_VAO);
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void HelloTriangle::destroyRectangle(){
    glDeleteVertexArrays(1, &RECTANGLE_VAO);
    glDeleteBuffers(1, &RECTANGLE_VBO);
    glDeleteBuffers(1, &RECTANGLE_EBO);
}


void HelloTriangle::execute(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    GLFWwindow *window = glfwCreateWindow(width, height, "learn openGL", nullptr, nullptr);
    
    if(window == nullptr){
        std::cout<<"Fail to create GLFW window"<<std::endl;
        glfwTerminate();
        return;
    }
    
    /*For Mac*/
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    /**/
    
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    
    if(glewInit()!=GLEW_OK){
        std::cout<<"failed to initialize GLEW"<<std::endl;
        glfwTerminate();
        return;
    }
    
    glViewport(0, 0, screenWidth, screenHeight);
    
    /**
            shader
     */
    
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout<<"Error::Shader::VERTEX::COMPILATION_FAILED\n"<<infoLog<<std::endl;
    }
    
    unsigned int fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(fragShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragShader);
    
    
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    
    if(!success){
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cout<<"Error::Shader::FRAGMENT::COMPILATION_FAILED\n"<<infoLog<<std::endl;
    }
    
    //shader program
    
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(__GLEW_AMD_shader_stencil_export, GL_LINK_STATUS, &success);
    
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout<<"Error::PROGRAM::LINK_FAILED\n"<<infoLog<<std::endl;
    }
        
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
    
//    createRectangle();
    std::cout<<"[HelloTriangle::execute] address of vertices "<<vertices<<", size of vertices : "<<sizeof(vertices)<<std::endl;

    createTriangle(vertices, sizeof(vertices),0);
    createTriangle(triangleAnotherVertices, sizeof(triangleAnotherVertices), 1);
    createTriangle(leftTriangleVertices, sizeof(leftTriangleVertices), 2);

    
    while(!glfwWindowShouldClose(window)){
        processInput(window);
        
        glfwPollEvents();
        
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);

//        drawRectangle();
        drawTriangle(0);
        drawTriangle(1);
        drawTriangle(2);
        
        
        glfwSwapBuffers(window);
    }
    
//    destroyRectangle();
    destroyTriangle(0);
    destroyTriangle(1);
    destroyTriangle(2);

    
    glDeleteProgram(shaderProgram);

    glfwTerminate();
}


