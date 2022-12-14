//
//  Colors.cpp
//  GLFWOpenGL
//
//  Created by Wongeun Cho on 2022/09/13.
//

#include "Colors.hpp"


void Colors::processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int Colors::createVertexShader(const char *vertexShader){
    unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    
    glShaderSource(vertexShaderID, 1, &vertexShader, NULL);
    glCompileShader(vertexShaderID);
    
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
    
    if(!success){
        glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
        std::cout<<"Error::Shader::VERTEX::COMPILATION_FAILED\n"<<infoLog<<std::endl;
        return -1;
    }
    
    return vertexShaderID;
    
}

int Colors::createFragmentShader(const char *fragmentShader)
{
    int fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(fragShaderID, 1, &fragmentShader, NULL);
    glCompileShader(fragShaderID);
    
    int success;
    char infoLog[512];

    glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &success);
    
    if(!success){
        glGetShaderInfoLog(fragShaderID, 512, NULL, infoLog);
        std::cout<<"Error::Shader::FRAGMENT::COMPILATION_FAILED\n"<<infoLog<<std::endl;
        return -1;
    }
    
    return fragShaderID;
}

int Colors::createProgram(const char *vertexShader, const char *fragmentShader){
    int success;
    char infoLog[512];

    int vertexShaderID = createVertexShader(vertexShader);
    int fragmentShaderID = createFragmentShader(fragmentShader);
    
    if(vertexShaderID<0 || fragmentShaderID<0){
        std::cout<<"Error::createShaderError\n"<<std::endl;
    }
    
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    
    glAttachShader(shaderProgram, vertexShaderID);
    glAttachShader(shaderProgram, fragmentShaderID);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(__GLEW_AMD_shader_stencil_export, GL_LINK_STATUS, &success);
    
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout<<"Error::PROGRAM::LINK_FAILED\n"<<infoLog<<std::endl;
        return -1;
    }
        
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    
    return shaderProgram;
}

void Colors::createCube(){
    
    glGenVertexArrays(1, &CUBE_VAO);
    glGenBuffers(1, &CUBE_VBO);

    glBindVertexArray(CUBE_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, CUBE_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind
    glBindVertexArray(0);
}

void Colors::createLamp(){
    glGenVertexArrays(1, &LIGHT_VAO);
    glBindVertexArray(LIGHT_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, CUBE_VBO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Colors::assignLightUniform(unsigned int shaderProgramID){
    glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, glm::value_ptr(objectColor));
    glUniform3fv(glGetUniformLocation(shaderProgramID, "lightColor"), 1, glm::value_ptr(lightColor));
}


void Colors::drawCube(){
    glBindVertexArray(CUBE_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Colors::drawLamp(){
    glBindVertexArray(LIGHT_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Colors::destroyCube(){
    glDeleteVertexArrays(1, &CUBE_VAO);
    glDeleteBuffers(1, &CUBE_VBO);
}

void Colors::destroyLamp(){
    glDeleteVertexArrays(1, &LIGHT_VAO);
}

void Colors::initMVPMatrix(unsigned int shaderProgramID){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model,  glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
//    model = glm::scale(model, glm::vec3(0.5f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));
    
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1 , GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "view"), 1 , GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1 , GL_FALSE, glm::value_ptr(projection));
}


void Colors::initLampMatrix(unsigned int shaderProgramID){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.2f, 1.0f, 2.0f)); //The sequence is very important srt.
    model = glm::rotate(model,  glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));
    
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "view"), 1 , GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1 , GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1 , GL_FALSE, glm::value_ptr(model));
}

void Colors::execute(){
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
    
    unsigned int shaderProgram = createProgram(vertexShaderSource, fragmentShaderSource);
    unsigned int lampShaderProgram = createProgram(vertexShaderSource, lampFragmentShaderSource);
    
    createCube();
    createLamp();
    
    glUseProgram(shaderProgram);    //before set uniform value into shader, don't forget use shader.
    assignLightUniform(shaderProgram);
    
    glEnable(GL_DEPTH_TEST);
    
    while(!glfwWindowShouldClose(window)){
        processInput(window);
        
        glfwPollEvents();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        initMVPMatrix(shaderProgram);
        drawCube();
        
        glUseProgram(lampShaderProgram);
        initLampMatrix(lampShaderProgram);
        drawLamp();
        
        
        glfwSwapBuffers(window);
    }
    
    destroyCube();
    destroyLamp();
    
    glDeleteProgram(shaderProgram);
    
    glfwTerminate();
}
