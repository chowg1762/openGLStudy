#include <iostream>

//GLEW

#define GLEW_STATIC
#include <GL/glew.h>    //should be before GLFW.

//GLFW
#include <GLFW/glfw3.h>

#include "hello_triangle.hpp"
#include "intro.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "transformation.hpp"

#include "coordinateSystem.hpp"
#include "coordinateSystem_3d.hpp"
#include "camera.hpp"

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

void executeTexture(){
    Texture texture;
    texture.execute();
}

void executeTransformation(){
    Transformation transformation;
    transformation.execute();
}

void executeCoordinateSystem(){
    CoordinateSystem coordinate;
    coordinate.execute();
}

void executedCoordinateSystem3D(){
    CoordinateSystem3D coordinate;
    coordinate.execute();
}

void executeCamera(){
    CameraSystem camera;
    camera.execute();
}

int main(){
//    executeIntro();
//    executeHelloTriangle();
//    executeShader();
//    executeTexture();
//    executeTransformation();
//    executeCoordinateSystem();
//    executedCoordinateSystem3D();
    executeCamera();
}


