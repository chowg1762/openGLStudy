//
//  LightCaster.cpp
//  GLFWOpenGL
//
//  Created by Wongeun Cho on 2022/09/18.
//

#include "LightCaster.hpp"

int LightCaster::createVertexShader(const char *vertexShader){
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

int LightCaster::createFragmentShader(const char *fragmentShader)
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

int LightCaster::createProgram(const char *vertexShader, const char *fragmentShader){
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


unsigned int LightCaster::createTexture(std::string filePath, bool isRGBA){
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



void LightCaster::createCube(){
    
    glGenVertexArrays(1, &CUBE_VAO);
    glGenBuffers(1, &CUBE_VBO);

    glBindVertexArray(CUBE_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, CUBE_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind
    glBindVertexArray(0);
}

void LightCaster::createLamp(){
    glGenVertexArrays(1, &LIGHT_VAO);
    glBindVertexArray(LIGHT_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, CUBE_VBO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void LightCaster::assignLightUniform(unsigned int shaderProgramID){

    glUniform1f(glGetUniformLocation(shaderProgramID, "material.shininess"), 32.0f);
    
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    
    glm::vec3 diffuseLightColor = lightColor * glm::vec3(0.5f);
    glm::vec3 ambientLightColor = diffuseLightColor * glm::vec3(0.2f);
    
//    glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);  //Directional Light 's light.direction

    glUniform3fv(glGetUniformLocation(shaderProgramID, "light.ambient"), 1, glm::value_ptr(ambientLightColor));
    glUniform3fv(glGetUniformLocation(shaderProgramID, "light.diffuse"), 1, glm::value_ptr(diffuseLightColor));
    glUniform3fv(glGetUniformLocation(shaderProgramID, "light.specular"), 1, glm::value_ptr(glm::vec3(1.f, 1.f, 1.f)));
    
    glUniform1f(glGetUniformLocation(shaderProgramID, "light.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(shaderProgramID, "light.linear"), 0.09f);
    glUniform1f(glGetUniformLocation(shaderProgramID, "light.quadratic"), 0.032f);
    
    glUniform3fv(glGetUniformLocation(shaderProgramID, "light.position"), 1, glm::value_ptr(CallbackUtils::mCameraPos));
    glUniform3fv(glGetUniformLocation(shaderProgramID, "light.direction"), 1, glm::value_ptr(CallbackUtils::mCameraFront));
    glUniform1f(glGetUniformLocation(shaderProgramID, "light.cutoff"), glm::cos(glm::radians(12.5f)));
    glUniform1f(glGetUniformLocation(shaderProgramID, "light.outCutoff"), glm::cos(glm::radians(17.5f)));


//    glUniform3fv(glGetUniformLocation(shaderProgramID, "light.direction"), 1, glm::value_ptr(lightDir));  //Directional Light 's light.direction

}


void LightCaster::drawCube(){
    glBindVertexArray(CUBE_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void LightCaster::drawLamp(){
    glBindVertexArray(LIGHT_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void LightCaster::destroyCube(){
    glDeleteVertexArrays(1, &CUBE_VAO);
    glDeleteBuffers(1, &CUBE_VBO);
}

void LightCaster::destroyLamp(){
    glDeleteVertexArrays(1, &LIGHT_VAO);
}

void LightCaster::initMVPMatrix(unsigned int shaderProgramID){
    glm::mat4 model = glm::mat4(1.0f);
    
    glm::mat view = glm::lookAt(CallbackUtils::mCameraPos, CallbackUtils::mCameraPos+CallbackUtils::mCameraFront, CallbackUtils::mCameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(CallbackUtils::fov), 800.0f / 600.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1 , GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "view"), 1 , GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1 , GL_FALSE, glm::value_ptr(projection));
}

void LightCaster::initModelMatrix(unsigned int shaderProgramID, int cubeIndex){
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, cubePositions[cubeIndex]);
        float angle = 20.0f * cubeIndex;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1 , GL_FALSE, glm::value_ptr(model));
}

void LightCaster::initViewMatrix(unsigned int shaderProgramID){
    glm::mat view = glm::lookAt(CallbackUtils::mCameraPos, CallbackUtils::mCameraPos+CallbackUtils::mCameraFront, CallbackUtils::mCameraUp);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "view"), 1 , GL_FALSE, glm::value_ptr(view));

}

void LightCaster::initProjectionMatrix(unsigned int shaderProgramID){
    glm::mat4 projection = glm::perspective(glm::radians(CallbackUtils::fov), 800.0f / 600.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1 , GL_FALSE, glm::value_ptr(projection));
}


void LightCaster::initLampMatrix(unsigned int shaderProgramID){
    glm::mat4 model = glm::mat4(1.0f);
    
    model = glm::translate(model, lightPos); //The sequence is very important srt.
    model = glm::scale(model, glm::vec3(0.2f));

    glm::mat view = glm::lookAt(CallbackUtils::mCameraPos, CallbackUtils::mCameraPos+CallbackUtils::mCameraFront, CallbackUtils::mCameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(CallbackUtils::fov), 800.0f / 600.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "view"), 1 , GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1 , GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1 , GL_FALSE, glm::value_ptr(model));
}

void LightCaster::execute(){
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
    
    unsigned int textureID = createTexture(containerImage, true);
    unsigned int specularTextureID = createTexture(containerSpecularImage, true);
    
    glUseProgram(shaderProgram);    //before set uniform value into shader, don't forget use shader.
    
    glEnable(GL_DEPTH_TEST);

    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(window, CallbackUtils::mouseCallback);
    glfwSetScrollCallback(window, CallbackUtils::scrollCallback);
    

    
    while(!glfwWindowShouldClose(window)){
        float currentTime = glfwGetTime();
        CallbackUtils::deltaTime = currentTime - CallbackUtils::lastFrameTime;
        CallbackUtils::lastFrameTime = currentTime;
        
        CallbackUtils::processInput(window);
        
        glfwPollEvents();
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        for(int i=0; i<10; i++){
            glUseProgram(shaderProgram);
            
            assignLightUniform(shaderProgram);
//            initMVPMatrix(shaderProgram);
            initModelMatrix(shaderProgram, i);
            initViewMatrix(shaderProgram);
            initProjectionMatrix(shaderProgram);
            
            glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
            glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(CallbackUtils::mCameraPos));
            glUniform1i(glGetUniformLocation(shaderProgram, "Material.diffuse"),0); //Material.diffuse 를 Texture 0번에 Mapping
            glUniform1i(glGetUniformLocation(shaderProgram, "Material.specular"),1);    //Material.specular 를 Texture 1번에 Mapping
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureID);  //GL_TEXTURE0 으로 Active 되어있는 GL_TEXTURE_2D 에 textureID Mapping.
        
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularTextureID);  //GL_TEXTURE1 으로 Active 되어있는 GL_TEXTURE_2D 에 textureID Mapping.

            drawCube();
        }
        
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
