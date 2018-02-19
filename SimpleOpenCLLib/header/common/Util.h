//
//  Util.hpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/5/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef Util_h
#define Util_h

#include <iostream>
#include <exception>
#include <string>
#include "common.h"

void getTime(TimeSpec* pTimeSpec);

void logAndThrow(std::string exceptionMessage);

class ScopeTimer {
public:
    ScopeTimer(TimeSpec* pTimeSpec);
    virtual ~ScopeTimer();
    
private:
    TimeSpec* m_pTimeSpec;
    TimeSpec m_start;
};

#endif /* Util_hpp */
