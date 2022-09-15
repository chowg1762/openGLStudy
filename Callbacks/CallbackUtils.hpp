//
//  CallbackUtils.hpp
//  GLFWOpenGL
//
//  Created by Wongeun Cho on 2022/09/15.
//

#ifndef CallbackUtils_hpp
#define CallbackUtils_hpp

#include <stdio.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class CallbackUtils{
    
    private :
    
        
    public :
   
    static inline glm::vec3 mCameraPos = glm::vec3(0.0f, 0.0f,  3.0f);
    static inline glm::vec3 mCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    static inline glm::vec3 mCameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

    static inline float lastFrameTime = 0.0f;
    static inline float deltaTime = 0.0f;


    static inline float lastX = 400, lastY = 300;
    static inline float yaw = 0.0f, pitch = 0.0f;
    static inline float fov = 45.0f;

    static inline bool firstMouse = true;
   
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void processInput(GLFWwindow *window);

    
};
#endif /* CallbackUtils_hpp */
