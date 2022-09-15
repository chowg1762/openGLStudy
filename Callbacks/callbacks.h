//
//  callbacks.h
//  GLFWOpenGL
//
//  Created by Wongeun Cho on 2022/09/15.
//

#ifndef callbacks_h
#define callbacks_h

#include <GLFW/glfw3.h>

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


#endif /* callbacks_h */
