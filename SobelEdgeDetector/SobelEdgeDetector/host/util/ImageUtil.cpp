//
//  ImageUtil.cpp
//  SobelEdgeDetector
//
//  Created by wws2003 on 4/22/18.
//  Copyright © 2018 hpg. All rights reserved.
//

#include "ImageUtil.h"
#include <fstream>

#define IMAGE_ELE_SWAP(a,b) {image_ele_t aux = a; a = b; b = aux;}
#define BITS_PER_PIXEL (8)

// Implement logic follows sample bmpfuncs.c
void ImageUtil::readImage(const char* imgFileName, BmpImageModel& outputModel) {
    // Open file
    std::fstream fs(imgFileName, std::iostream::in | std::iostream::binary);
    
    image_size_t offset;
    image_size_t width;
    image_size_t height;
    
    // Offset
    fs.seekg(10);
    fs.read((char*)&offset, 4);
    
    // Width, height
    fs.seekg(18);
    fs >> width;
    fs.read((char*)&width, 4);
    fs >> height;
    fs.read((char*)&height, 4);
    
    // To specify number of pixel each row to be thrown away
    int padding = width % 4;
    if (padding != 0) {
        padding = 4 - padding;
    }

    // Pixel array (read from upside-down into right-side up)
    uchar* tmpImageData = new uchar[width * height];
    
    // NOTE bitmaps are stored in upside-down raster order.  So we begin
    // reading from the bottom left pixel, then going from left-to-right,
    // read from the bottom to the top of the image.  For image analysis,
    // we want the image to be right-side up, so we'll modify it here.
    
    // First we read the image in upside-down
    
    // Read in the actual image
    fs.seekg(offset);
    for(image_size_t i = 0; i < height; i++) {
        uchar tmp;
        // add actual data to the image
        for (image_size_t j = 0; j < width; j++) {
            // Only support BITS_PER_PIXEL = 8 ?
            fs >> tmp;
            tmpImageData[i * width + j] = tmp;
        }
        // For the bmp format, each row has to be a multiple of 4,
        // so I need to read in the junk data (remaing data of the row) and throw it away
        for(int j = 0; j < padding; j++) {
            fs >> tmp;
        }
    }
    
    // Flip over (so that bottom row comes to top and vice versa)
    for (image_size_t i = 0; i < height / 2; i++) {
        image_size_t flipRow = height - 1 - i;
        for (image_size_t j = 0; j < width; j++) {
            // Swap
            IMAGE_ELE_SWAP(tmpImageData[i * width + j], tmpImageData[flipRow * width + j]);
        }
    }
    
    // To host-used model
    image_ele_t* imageDataBuffer = new image_ele_t[width * height];
    for (image_size_t i = 0; i < height; i++) {
        for (image_size_t j = 0; j < width; j++) {
            imageDataBuffer[i * width + j] = (float)tmpImageData[i * width + j];
        }
    }
    
    // Release temp
    delete [] tmpImageData;
    
    // Set to result
    outputModel.m_width = width;
    outputModel.m_height = height;
    outputModel.m_data.m_size = width * height;
    outputModel.m_data.m_buffer = imageDataBuffer;
    outputModel.m_fileMeta.m_offset = offset;
}

void ImageUtil::writeImage(const char* fileName, const BmpImageModel& inputModel) {
    // TODO Implement
    std::fstream fs(fileName, std::iostream::out | std::iostream::binary);
    
    image_size_t width = inputModel.m_width;
    image_size_t height = inputModel.m_height;
    image_ele_t* imageBuffer = inputModel.m_data.m_buffer;
    image_size_t offset = inputModel.m_fileMeta.m_offset;
    
    // Pixel array
    size_t imageDataSize = ((BITS_PER_PIXEL * width + 31) / 32) * 4;
    uchar* tmpImageData = new uchar[imageDataSize];
    
    // Copy from host-used model
    int padding = width % 4;
    if(padding != 0) {
        padding = 4 - padding;
    }
    for (image_size_t i = 0; i < height; i++) {
        for (image_size_t j = 0; j < width; j++) {
            tmpImageData[i * width + j] = (uchar)imageBuffer[i * width + j];
        }
        // Add padding (just to keep data layout)
        image_size_t rowOffset = (i + 1) * width;
        for (int j = 0; j < padding; j++) {
            tmpImageData[rowOffset + j] = 1;
        }
    }
    
    // Flip over (so that bottom row comes to top and vice versa)
    for (image_size_t i = 0; i < height / 2; i++) {
        image_size_t flipRow = height - 1 - i;
        for (image_size_t j = 0; j < width; j++) {
            // Swap
            IMAGE_ELE_SWAP(tmpImageData[i * width + j], tmpImageData[flipRow * width + j]);
        }
    }
    
    // Write to output file
    
    // TODO Header
    fs.seekg(10);
    fs.write((const char*)offset, 4);
    
    // Width, height
    fs.seekg(18);
    fs.write((const char*)width, 4);
    fs.write((const char*)height, 4);
    
    // Data
    fs.seekg(offset);
    fs.write((const char*)tmpImageData, imageDataSize);
    
    delete [] tmpImageData;
}