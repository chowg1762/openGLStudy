//
//  CallbackUtils.cpp
//  GLFWOpenGL
//
//  Created by Wongeun Cho on 2022/09/15.
//

#include "CallbackUtils.hpp"

void CallbackUtils::mouseCallback(GLFWwindow* window, double xpos, double ypos){
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xOffset = xpos - lastX;
    float yOffset = lastY-ypos;
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


void CallbackUtils::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}



void CallbackUtils::processInput(GLFWwindow *window){
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
