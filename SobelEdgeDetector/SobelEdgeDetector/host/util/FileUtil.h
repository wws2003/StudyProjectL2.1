//
//  FileUtil.h
//  SobelEdgeDetector
//
//  Created by wws2003 on 4/29/18.
//  Copyright © 2018 hpg. All rights reserved.
//

#ifndef FileUtil_h
#define FileUtil_h

#include <cstdlib>
#include "Common.h"

class FileUtil {
public:
    /**
     * Read the file content given the file full path
     */
    static const char* readFile(const char* filePath);
};

#endif /* FileUtil_h */
