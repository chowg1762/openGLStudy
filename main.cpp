#include <iostream>

//GLEW

#define GLEW_STATIC
#include <GL/glew.h>    //should be before GLFW.

//GLFW
#include <GLFW/glfw3.h>

#include "hello_triangle.hpp"
#include "intro.hpp"
#include "shader.hpp"

void executeHelloTriangle(){
    HelloTriangle helloTriangle;
    helloTriangle.execute();
}

void executeIntro(){
    Intro intro;
    intro.execute();
}

void executeShader(){
    Shader shader;
    shader.execute();
}

int main(){
//    executeIntro();
//    executeHelloTriangle();
    executeShader();
}


