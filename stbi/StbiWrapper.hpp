//
//  StbiWrapper.hpp
//  GLFWOpenGL
//
//  Created by Wongeun Cho on 2022/08/17.
//

#ifndef StbiWrapper_hpp
#define StbiWrapper_hpp

#include <stdio.h>

class StbiWrapper{
public:
    static void setYFlip(bool yFlip);

    static unsigned char* loadImage(const char* filePath, int* width, int* height, int* numOfChannel, int reqComp);

    static void freeImage(unsigned char* data);
};


#endif /* StbiWrapper_hpp */
