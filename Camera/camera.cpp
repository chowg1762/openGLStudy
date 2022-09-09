//
//  camera.cpp
//  GLFWOpenGL
//
//  Created by Wongeun Cho on 2022/09/09.
//

#include "camera.hpp"

glm::vec3 mCameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 mCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 mCameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

float lastFrameTime = 0.0f;
float deltaTime = 0.0f;

float lastX = 400, lastY = 300;
float yaw = 0.0f, pitch = 0.0f;
float fov = 45.0f;

bool firstMouse = true;

unsigned int CameraSystem::createTexture(std::string filePath, bool isRGBA){
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

void CameraSystem::processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    const float cameraSpeed = 2.5f*deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
       mCameraPos += cameraSpeed * mCameraFront;
   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
       mCameraPos -= cameraSpeed * mCameraFront;
   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
       mCameraPos -= glm::normalize(glm::cross(mCameraFront, mCameraUp)) * cameraSpeed;
   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
       mCameraPos += glm::normalize(glm::cross(mCameraFront, mCameraUp)) * cameraSpeed;
}

int CameraSystem::createVertexShader(const char *vertexShader){
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

int CameraSystem::createFragmentShader(const char *fragmentShader)
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

int CameraSystem::createProgram(const char *vertexShader, const char *fragmentShader){
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

void CameraSystem::createCube(){
    
    glGenVertexArrays(1, &CUBE_VAO);
    glGenBuffers(1, &CUBE_VBO);

    glBindVertexArray(CUBE_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, CUBE_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind
    glBindVertexArray(0);
}

void CameraSystem::activateTexture(unsigned int textureID, int textureUnit){
    glActiveTexture(GL_TEXTURE0+textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}


void CameraSystem::drawCube(){
    glBindVertexArray(CUBE_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void CameraSystem::destroyCube(){
    glDeleteVertexArrays(1, &CUBE_VAO);
    glDeleteBuffers(1, &CUBE_VBO);
}


void CameraSystem::setMatrix(unsigned int shaderProgramID){
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
    
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "transformMatrix"), 1 , GL_FALSE, glm::value_ptr(trans));

}

void CameraSystem::doTransform(unsigned int shaderProgramID){
    glm::mat4 trans = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "transformMatrix"), 1 , GL_FALSE, glm::value_ptr(trans));
}

void CameraSystem::initMVPMatrix(unsigned int shaderProgramID){
    glm::mat4 model = glm::mat4(1.0f);
//    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1 , GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "view"), 1 , GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1 , GL_FALSE, glm::value_ptr(projection));
}

void CameraSystem::initMVPMatrices(unsigned int shaderProgramID, int cubeIndex){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cubePositions[cubeIndex]);
    float angle = 20.0f * (cubeIndex+1);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

    //    
//    const float radius = 10;
//    float camX = glm::sin((float)glfwGetTime()) * radius;
//    float camZ = glm::cos((float)glfwGetTime()) * radius;
    
    
//    glm::mat4 view = glm::lookAt(glm::vec3(camX, 0.0f, camZ),   //camera pos
//                                 glm::vec3(0.0f, 0.0f, 0.0f),   //target pos
//                                 glm::vec3(0.0f, 1.0f, 0.0f));  // up vector
    
    glm::mat view = glm::lookAt(mCameraPos, mCameraPos+mCameraFront, mCameraUp);
    
    glm::mat4 projection;
//    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    
    projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
    
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1 , GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "view"), 1 , GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1 , GL_FALSE, glm::value_ptr(projection));

}

void CameraSystem::initMVPMatricesForRotating(unsigned int shaderProgramID, int cubeIndex){
    glm::mat4 model = glm::mat4(1.0f);
    
    double currentTime = glfwGetTime();
    if(cubeIndex==0)
        model = glm::translate(model, twoCubes[cubeIndex]);
    else
        model = glm::translate(model,glm::vec3(sin(currentTime)*4.0f, 0.0, cos(currentTime)));
    
    std::cout<<"glGetTime : "<<currentTime<<", sin(currentTIme) = " << sin(currentTime) << ", cos(currentTime) = "<<cos(currentTime)<<std::endl;
    
    float angle = 20.0f * ((cubeIndex+1)*10);
    if(cubeIndex==0)
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    else
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
    
    
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1 , GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "view"), 1 , GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1 , GL_FALSE, glm::value_ptr(projection));

}


void CameraSystem::drawCubes(unsigned int shaderProgram){
    for(int i=0; i<10; i++){
        initMVPMatrices(shaderProgram, i);
//        initMVPMatricesForRotating(shaderProgram, i);
        drawCube();
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xOffset = xpos - lastX;
    float yOffset = ypos - lastY;
    lastX = xpos;
    lastY = ypos;
    
    const float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;
    
    yaw += xOffset;
    pitch += yOffset;
    
    if(pitch > 89.0f)
         pitch = 89.0f;
     if(pitch < -89.0f)
         pitch = -89.0f;

    
    glm::vec3 direction;
    
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    
    mCameraFront = glm::normalize(direction);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}



void CameraSystem::execute(){
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

    createCube();
    
    glUseProgram(shaderProgram);    //before set uniform value into shader, don't forget use shader.

    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"),0);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture2"),1);
    
    glEnable(GL_DEPTH_TEST);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    
    while(!glfwWindowShouldClose(window)){
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
        
        processInput(window);
        
        glfwPollEvents();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        
        activateTexture(textureID,0);
        activateTexture(texture2ID,1);
        
        drawCubes(shaderProgram);
        
        
        glfwSwapBuffers(window);
    }
    
    destroyCube();
    
    glDeleteProgram(shaderProgram);
    
    glfwTerminate();
}

