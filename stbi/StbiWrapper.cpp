//
//  StbiWrapper.cpp
//  GLFWOpenGL
//
//  Created by Wongeun Cho on 2022/08/17.
//

#include "StbiWrapper.hpp"

//STB_IMAGE
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void StbiWrapper::setYFlip(bool yFlip){
    stbi_set_flip_vertically_on_load(yFlip); //flip y-axis
}

unsigned char* StbiWrapper::loadImage(const char* filePath, int* width, int* height, int* numOfChannel, int reqComp){
    return stbi_load(filePath, width, height, numOfChannel, reqComp);
}

void StbiWrapper::freeImage(unsigned char* data){
    stbi_image_free(data);
}
