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

std::string FileUtil::readFile(const char* filePath) {
    std::ifstream fstream(filePath, std::ios::in);
    if (!fstream.is_open()) {
        std::cerr << "Failed to open CL source file for reading: " << filePath << std::endl;
        throw std::runtime_error("Failed to open CL source file for reading");
    }
    
    std::ostringstream oss;
    oss << fstream.rdbuf();
    return oss.str();
}