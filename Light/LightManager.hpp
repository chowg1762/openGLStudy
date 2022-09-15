//
//  LightManager.hpp
//  GLFWOpenGL
//
//  Created by Wongeun Cho on 2022/09/14.
//

#ifndef LightManager_hpp
#define LightManager_hpp

#include <stdio.h>
#include "Colors.hpp"
#include "BasicLighting.hpp"
#include "Materials.hpp"

class LightManager{
private:
    static void colorExecute();
    static void basicLightingExecute();
    static void materialsExecute();
public:
    static void execute();
};

#endif /* LightManager_hpp */
