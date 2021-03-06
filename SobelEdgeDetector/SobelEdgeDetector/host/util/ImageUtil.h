//
//  ImageUtil.h
//  SobelEdgeDetector
//
//  Created by wws2003 on 4/22/18.
//  Copyright © 2018 hpg. All rights reserved.
//

#ifndef ImageUtil_h
#define ImageUtil_h

#include "Common.h"
#include "BmpImageModel.h"
#include <cstdlib>

/**
 * Class to provide basic IO functions against image file
 */
class ImageUtil {
public:
    /**
     * Read data from file
     * @param imgFileName Image file name (full path)
     * @param outputModel Reference of image model to read data into
     */
    static void readImage(const char* imgFileName, BmpImageModel& outputModel);
    
    /**
     * Output data to file
     * @param fileName Output image file name (full path)
     * @param inputModel Model of input image to write
     */
    static void writeImage(const char* fileName, const BmpImageModel& inputModel);
};

#endif 

/* ImageUtil_h */
