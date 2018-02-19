//
//  SimpleCLExecutorImpl.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 3/29/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef SimpleCLExecutorImpl_h
#define SimpleCLExecutorImpl_h

#include "ISimpleCLExecutor.h"
#include "ISimpleCLProgram.h"
#include "ISimpleCLProgramFactory.h"
#include <vector>
#include <map>
#include <string>

class SimpleCLExecutorImpl: public ISimpleCLExecutor {
public:
    
    //Also implicitly transfer owner of context instance to this SimpleCLProgramImpl instance
    SimpleCLExecutorImpl(cl_context context, SimpleCLProgramFactoryPtr pSimpleCLProgramFactory);
    
    virtual ~SimpleCLExecutorImpl();
    
    //Any void function would throw exception if doesn't successfully operated
    
    //Should only be called in factory class, just to avoid doing too many things in constructor
    void setup();
    
    //Implement
    void loadProgram(const char* clSourceFilePath, const ProgramPrototype& programPrototype);
    
    //Implement
    submission_t submitOnce(const char* kernelName,
                    const WorkDims& workDims,
                    const HostInputParams& inputParams);
    
    //Implement
    void getResult(const submission_t& submisson, 
                   HostOutputParams& outputs,
                   TimeSpec* pTimeSpec);
    
private:
    typedef cl_event* EventPtr;
    typedef std::map<submission_t, EventPtr> EventMap;
    
    void initCommandQueueOnFirstDevice();
    
    void computeRunTime(EventPtr pEvent, TimeSpec* pTimeSpec);
    
    void releaseCommandQueue();
    
    void releaseCurrentProgram();
    
    void releaseContext();
    
    cl_context m_context = 0;
    cl_command_queue m_commandQueue = 0;
    
    cl_device_id m_deviceIdInUsed = 0;
    
    SimpleCLProgramFactoryPtr m_pSimpleCLProgramFactory;
    SimpleCLProgramPtr m_pCurrentCLProgram = NULL;
    
    class CommandEventPool {
    public:
        CommandEventPool(){};
        
        virtual ~CommandEventPool();
        
        submission_t submitCommandEvent(EventPtr pEvent);
        
        EventPtr getCommandEventForSubmission(submission_t submission);
        
        void releaseSubmission(submission_t submission);
        
    private:
        EventMap m_submissionEventMap;
    };
    
    CommandEventPool m_commandEventPool;
};

typedef SimpleCLExecutorImpl* SimpleCLExecutorImplPtr;

#endif /* SimpleCLExecutorImpl_h */
