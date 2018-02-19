//
//  Util.cpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/5/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#include "Util.h"
#include <ctime>
#include <iostream>
#include <mach/clock.h>
#include <mach/mach.h>

void logAndThrow(std::string exceptionMessage) {
    std::cerr << exceptionMessage << std::endl;
    throw std::runtime_error(exceptionMessage);
}

void getTime(TimeSpec* pTimeSpec) {
    clock_serv_t cclock;
    mach_timespec_t mts;
    
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    
    pTimeSpec->tv_sec = mts.tv_sec;
    pTimeSpec->tv_nsec = mts.tv_nsec;
}

ScopeTimer::ScopeTimer(TimeSpec* pTimeSpec) {
    getTime(&m_start);
    m_pTimeSpec = pTimeSpec;
}

ScopeTimer::~ScopeTimer() {
    TimeSpec end;
    getTime(&end);
    
    if (m_pTimeSpec != NULL) {
        long deltaNSec = (end.tv_sec - m_start.tv_sec) * 1e9 + end.tv_nsec - m_start.tv_nsec;
        m_pTimeSpec->tv_sec = deltaNSec / 1e9;
        m_pTimeSpec->tv_nsec = deltaNSec - (m_pTimeSpec->tv_sec) * 1e9;
    }
}