//
//  Common.h
//  SobelEdgeDetector
//
//  Created by wws2003 on 4/22/18.
//  Copyright © 2018 hpg. All rights reserved.
//

#ifndef Common_h
#define Common_h

#include <vector>

#define HOST_SWAP(a,b) {int aux = a; a = b; b = aux;}

typedef unsigned char uchar;

// Data type for image element
typedef float image_ele_t;

// Data type for image size (width/height)
typedef size_t image_size_t;

// Data type for value in filter array
typedef float filter_ele_t;
typedef std::vector<filter_ele_t> filter_t;

class IImageFilterApplier;
typedef IImageFilterApplier* ImageFilterApplierPtr;

#endif /* Common_h */
