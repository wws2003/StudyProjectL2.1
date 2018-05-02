//
//  IImageFilterApplier.h
//  SobelEdgeDetector
//
//  Created by wws2003 on 4/29/18.
//  Copyright © 2018 hpg. All rights reserved.
//

#ifndef IImageFilterApplier_h
#define IImageFilterApplier_h

#include "Common.h"
#include "BmpImageModel.h"

/**
 * Abstract class to apply filter to image model object
 */
class IImageFilterApplier {
public:
    IImageFilterApplier(){};
    virtual ~IImageFilterApplier(){};
    
    /**
     * Apply filter to the input image and get output written in output image
     * @param inImage
     * @param filter
     * @param outImage
     */
    virtual void filter(const BmpImageModel& inImage, const filter_t& filter, BmpImageModel& outImage) = 0;
};

#endif /* IImageFilterApplier_h */
