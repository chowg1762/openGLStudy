//
//  IntroFactory.cpp
//  GLFWOpenGL
//
//  Created by Wongeun Cho on 2022/09/13.
//

#include "IntroManager.hpp"


void IntroManager::executeHelloTriangle(){
    HelloTriangle helloTriangle;
    helloTriangle.execute();
}

void IntroManager::executeIntro(){
    Intro intro;
    intro.execute();
}

void IntroManager::executeShader(){
    Shader shader;
    shader.execute();
}

void IntroManager::executeTexture(){
    Texture texture;
    texture.execute();
}

void IntroManager::executeTransformation(){
    Transformation transformation;
    transformation.execute();
}

void IntroManager::executeCoordinateSystem(){
    CoordinateSystem coordinate;
    coordinate.execute();
}

void IntroManager::executedCoordinateSystem3D(){
    CoordinateSystem3D coordinate;
    coordinate.execute();
}

void IntroManager::executeCamera(){
    CameraSystem camera;
    camera.execute();
}

void IntroManager::execute(){
//    executeIntro();
//    executeHelloTriangle();
//    executeShader();
//    executeTexture();
//    executeTransformation();
//    executeCoordinateSystem();
//    executedCoordinateSystem3D();
    executeCamera();
}


