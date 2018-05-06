//
//  BmpImageModel.h
//  SobelEdgeDetector
//
//  Created by wws2003 on 4/22/18.
//  Copyright © 2018 hpg. All rights reserved.
//

#ifndef BmpImageModel_h
#define BmpImageModel_h

#include <cstdlib>
#include "Common.h"

struct BmpImageData {
    // Image data capacity
    size_t m_size;
    // Image data
    image_ele_t* m_buffer;
};

struct BmpFileMeta {
    // Image pixel array offset
    image_size_t m_offset;
    // Header data
    char* m_headerRawData;
    // TODO Add more attributes if needed
};

struct BmpImageModel {
    // Width
    image_size_t m_width;
    // Height
    image_size_t m_height;
    // Data
    BmpImageData m_data;
    // File metadata
    BmpFileMeta m_fileMeta;
};

#endif /* BmpImageModel_h */
