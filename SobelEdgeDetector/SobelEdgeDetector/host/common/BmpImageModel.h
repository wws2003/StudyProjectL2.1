//
//  BmpImageModel.h
//  SobelEdgeDetector
//
//  Created by wws2003 on 4/22/18.
//  Copyright © 2018 hpg. All rights reserved.
//

#ifndef BmpImageModel_h
#define BmpImageModel_h

#include "Common.h"

struct BmpImageModel {
public:
    // Width
    image_size_t m_width;
    // Height
    image_size_t m_height;
    // Data
    image_ele_t m_data;
};

#endif /* BmpImageModel_h */
