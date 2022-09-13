//
//  IntroFactory.hpp
//  GLFWOpenGL
//
//  Created by Wongeun Cho on 2022/09/13.
//

#ifndef IntroFactory_hpp
#define IntroFactory_hpp

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

class IntroManager{
private:
    static void executeHelloTriangle();
    static void executeIntro();
    static void executeShader();
    static void executeTexture();
    static void executeTransformation();
    static void executeCoordinateSystem();
    static void executedCoordinateSystem3D();
    static void executeCamera();
public:
    static void execute();
};
#endif /* IntroFactory_hpp */
