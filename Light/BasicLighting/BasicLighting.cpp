//
//  BasincLighting.cpp
//  GLFWOpenGL
//
//  Created by Wongeun Cho on 2022/09/14.
//

#include "BasicLighting.hpp"



glm::vec3 mCameraPos_light   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 mCameraFront_light = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 mCameraUp_light    = glm::vec3(0.0f, 1.0f,  0.0f);

float lastFrameTime_light = 0.0f;
float deltaTime_light = 0.0f;

float lastX_light = 400, lastY_light = 300;
float yaw_light = 0.0f, pitch_light = 0.0f;
float fov_light = 45.0f;

bool firstMouse_light = true;


void mouse_callback_light(GLFWwindow* window, double xpos, double ypos){
    if (firstMouse_light)
    {
        lastX_light = xpos;
        lastY_light = ypos;
        firstMouse_light = false;
    }
    
    float xOffset = xpos - lastX_light;
    float yOffset = lastY_light-ypos;
    lastX_light = xpos;
    lastY_light = ypos;
    
    const float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;
    
    yaw_light += xOffset;
    pitch_light += yOffset;
    
    if(pitch_light > 89.0f)
         pitch_light = 89.0f;
     if(pitch_light < -89.0f)
         pitch_light = -89.0f;

    
    glm::vec3 direction;
    
    direction.x = cos(glm::radians(yaw_light)) * cos(glm::radians(pitch_light));
    direction.y = sin(glm::radians(pitch_light));
    direction.z = sin(glm::radians(yaw_light)) * cos(glm::radians(pitch_light));
    
    mCameraFront_light = glm::normalize(direction);
}


void scroll_callback_light(GLFWwindow* window, double xoffset, double yoffset)
{
    fov_light -= (float)yoffset;
    if (fov_light < 1.0f)
        fov_light = 1.0f;
    if (fov_light > 45.0f)
        fov_light = 45.0f;
}



void BasicLighting::processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    const float cameraSpeed = 2.5f*deltaTime_light;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
       mCameraPos_light += cameraSpeed * mCameraFront_light;
   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
       mCameraPos_light -= cameraSpeed * mCameraFront_light;
   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
       mCameraPos_light -= glm::normalize(glm::cross(mCameraFront_light, mCameraUp_light)) * cameraSpeed;
   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
       mCameraPos_light += glm::normalize(glm::cross(mCameraFront_light, mCameraUp_light)) * cameraSpeed;
}

int BasicLighting::createVertexShader(const char *vertexShader){
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

int BasicLighting::createFragmentShader(const char *fragmentShader)
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

int BasicLighting::createProgram(const char *vertexShader, const char *fragmentShader){
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

void BasicLighting::createCube(){
    
    glGenVertexArrays(1, &CUBE_VAO);
    glGenBuffers(1, &CUBE_VBO);

    glBindVertexArray(CUBE_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, CUBE_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind
    glBindVertexArray(0);
}

void BasicLighting::createLamp(){
    glGenVertexArrays(1, &LIGHT_VAO);
    glBindVertexArray(LIGHT_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, CUBE_VBO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void BasicLighting::assignLightUniform(unsigned int shaderProgramID){
    glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, glm::value_ptr(objectColor));
    glUniform3fv(glGetUniformLocation(shaderProgramID, "lightColor"), 1, glm::value_ptr(lightColor));
}


void BasicLighting::drawCube(){
    glBindVertexArray(CUBE_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void BasicLighting::drawLamp(){
    glBindVertexArray(LIGHT_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void BasicLighting::destroyCube(){
    glDeleteVertexArrays(1, &CUBE_VAO);
    glDeleteBuffers(1, &CUBE_VBO);
}

void BasicLighting::destroyLamp(){
    glDeleteVertexArrays(1, &LIGHT_VAO);
}

void BasicLighting::initMVPMatrix(unsigned int shaderProgramID){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.5f));
//    model = glm::rotate(model,  glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

//    glm::mat4 view = glm::mat4(1.0f);
//    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
//
//    glm::mat4 projection;
//    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    
    glm::mat view = glm::lookAt(mCameraPos_light, mCameraPos_light+mCameraFront_light, mCameraUp_light);
    glm::mat4 projection = glm::perspective(glm::radians(fov_light), 800.0f / 600.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1 , GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "view"), 1 , GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1 , GL_FALSE, glm::value_ptr(projection));
}


void BasicLighting::initLampMatrix(unsigned int shaderProgramID){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.2f));
//    model = glm::rotate(model,  glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    model = glm::translate(model, lightPos); //The sequence is very important srt.
//
//    glm::mat4 view = glm::mat4(1.0f);
//    glm::mat4 projection = glm::mat4(1.0f);
//
    
    glm::mat view = glm::lookAt(mCameraPos_light, mCameraPos_light+mCameraFront_light, mCameraUp_light);
    glm::mat4 projection = glm::perspective(glm::radians(fov_light), 800.0f / 600.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "view"), 1 , GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1 , GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1 , GL_FALSE, glm::value_ptr(model));
}

void BasicLighting::execute(){
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback_light);
    glfwSetScrollCallback(window, scroll_callback_light);

    
    while(!glfwWindowShouldClose(window)){
        float currentTime = glfwGetTime();
        deltaTime_light = currentTime - lastFrameTime_light;
        lastFrameTime_light = currentTime;
        
        processInput(window);
        
        glfwPollEvents();
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        initMVPMatrix(shaderProgram);
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(mCameraPos_light));

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
