//
//  transformation.cpp
//  GLFWOpenGL
//
//  Created by Wongeun Cho on 2022/08/17.
//

#include "transformation.hpp"



unsigned int Transformation::createTexture(std::string filePath, bool isRGBA){
    unsigned int textureID;
    int width, height, nrChannels;
    
    StbiWrapper::setYFlip(true); //flip y-axis
    
    unsigned char* data =  StbiWrapper::loadImage((const char*)filePath.c_str(), &width, &height, &nrChannels,0);

    glGenTextures(1, &textureID);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    if (data)
    {
        if(isRGBA){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else{
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    
    StbiWrapper::freeImage(data);

    return textureID;
}



void Transformation::processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int Transformation::createVertexShader(const char *vertexShader){
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

int Transformation::createFragmentShader(const char *fragmentShader)
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

int Transformation::createProgram(const char *vertexShader, const char *fragmentShader){
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


void Transformation::createRectangle(){
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
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind
    glBindVertexArray(0);

}

void Transformation::activateTexture(unsigned int textureID, int textureUnit){
    glActiveTexture(GL_TEXTURE0+textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Transformation::drawRectangle(unsigned int textureID){
    glBindVertexArray(RECTANGLE_VAO);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Transformation::destroyRectangle(){
    glDeleteVertexArrays(1, &RECTANGLE_VAO);
    glDeleteBuffers(1, &RECTANGLE_VBO);
    glDeleteBuffers(1, &RECTANGLE_EBO);
}

void Transformation::setMatrix(unsigned int shaderProgramID){
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
    
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "transformMatrix"), 1 , GL_FALSE, glm::value_ptr(trans));

}

void Transformation::rotateRectangleEveryFrame(unsigned int shaderProgramID){
    glm::mat4 trans = glm::mat4(1.0f);
    
    trans = glm::translate(trans, glm::vec3(0.5f,-0.5f,0.0f));
    trans = glm::rotate(trans, (-1)*(float)glfwGetTime() , glm::vec3(0.0f, 0.0f, 1.0f));
//    trans = glm::translate(trans, glm::vec3(0.5f,-0.5f,0.0f));

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "transformMatrix"), 1 , GL_FALSE, glm::value_ptr(trans));

}

void Transformation::execute(){
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
    
    
    
    unsigned int shaderProgram = createProgram(vertexShaderSource, fragmentShaderWithTwoTextures);
    
    std::string firstImage = "/Users/wongeuncho/Desktop/Dev/image/brick_wall.jpeg";
    std::string secondImage = "/Users/wongeuncho/Desktop/Dev/image/awesomeface.png";
    
    unsigned int textureID = createTexture(firstImage, false);
    unsigned int texture2ID = createTexture(secondImage, true);
    
    std::cout<<"texture1ID = "<<textureID<<", texture2ID = "<<texture2ID<<std::endl;

    createRectangle();
    
    glUseProgram(shaderProgram);    //before set uniform value into shader, don't forget use shader.

    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"),0);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture2"),1);
//    setMatrix(shaderProgram);
    
    while(!glfwWindowShouldClose(window)){
        processInput(window);
        
        glfwPollEvents();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        rotateRectangleEveryFrame(shaderProgram);
        
        activateTexture(textureID,0);
        activateTexture(texture2ID,1);
        
        drawRectangle(textureID);
        
        glfwSwapBuffers(window);
    }
    
    destroyRectangle();
    
    glDeleteProgram(shaderProgram);
    
    glfwTerminate();
}
