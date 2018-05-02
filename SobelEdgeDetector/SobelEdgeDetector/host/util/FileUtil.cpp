//
//  FileUtil.cpp
//  SobelEdgeDetector
//
//  Created by wws2003 on 4/29/18.
//  Copyright © 2018 hpg. All rights reserved.
//

#include "FileUtil.h"
#include <iostream>
#include <exception>
#include <fstream>
#include <sstream>

const char* FileUtil::readFile(const char* filePath) {
    std::ifstream kernelFile(filePath, std::ios::in);
    if (!kernelFile.is_open()) {
        std::cerr << "Failed to open CL source file for reading: " << filePath << std::endl;
        throw std::runtime_error("Failed to open CL source file for reading");
    }
    
    std::ostringstream oss;
    oss << kernelFile.rdbuf();
    std::string srcStdStr = oss.str();
    
    kernelFile.close();
    
    return srcStdStr.data();
}