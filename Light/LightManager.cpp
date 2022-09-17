//
//  LightManager.cpp
//  GLFWOpenGL
//
//  Created by Wongeun Cho on 2022/09/14.
//

#include "LightManager.hpp"

void LightManager::colorExecute(){
    Colors color;
    color.execute();
}

void LightManager::basicLightingExecute(){
    BasicLighting basicLighting;
    basicLighting.execute();
}

void LightManager::materialsExecute(){
    Materials materials;
    materials.execute();
}

void LightManager::lightingMapExecute(){
    LightingMap lightingmap;
    lightingmap.execute();
}
void LightManager::execute(){
//    colorExecute();
//    basicLightingExecute();
//    materialsExecute();
    lightingMapExecute();
}
