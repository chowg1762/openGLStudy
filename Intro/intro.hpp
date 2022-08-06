//
//  intro.hpp
//  GLFW OpenGL
//
//  Created by Wongeun Cho on 2022/08/04.
//

#ifndef intro_hpp
#define intro_hpp

#include <stdio.h>


#include <stdio.h>
#include <iostream>

//GLEW

#define GLEW_STATIC
#include <GL/glew.h>    //should be before GLFW.

//GLFW
#include <GLFW/glfw3.h>

class Intro{
public:
    void execute();

private:
    const GLint width = 800, height = 600;

    void processInput(GLFWwindow *window);
};

#endif /* intro_hpp */
