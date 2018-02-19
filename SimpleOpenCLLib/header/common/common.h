//
//  common.h.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/2/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef common_h
#define common_h

#include <vector>
#include <OpenCL/opencl.h>
#include <map>
#include <string>
#include <ctime>

//------------------For input of CLExecutor------------------//

struct HostBuffer {
    void* m_data;
    size_t m_arraySize;
    size_t m_elementSize;
};

enum ParamType {
    PT_GLOBAL_IN,
    PT_GLOBAL_OUT,
    PT_GLOBAL_INOUT,
    PT_LOCAL,
    PT_CONSTANT
    //TODO Add more types
};

typedef std::vector<ParamType> ParamTypes;

struct HostBufferExt : public HostBuffer {
    ParamType m_paramType;
};

typedef std::vector<HostBufferExt> HostInputParams;
typedef std::vector<HostBuffer> HostOutputParams;

typedef long submission_t;
typedef struct timespec TimeSpec;

class IHostBufferSource {
public:
    virtual ~IHostBufferSource(){};
    
    virtual void toHostInputBuffer(ParamType paramType, HostBufferExt& inputBuffer, bool toAllocateMem = false) const = 0;
    
    virtual void toHostOutputBuffer(HostBuffer& outputBuffer) const = 0;
    
    //virtual void loadFromHostOutputBuffer(const HostBuffer& outputBuffer) = 0;
};

typedef IHostBufferSource* HostBufferSourcePtr;

typedef const IHostBufferSource* ConstHostBufferSourcePtr;

typedef std::vector<ConstHostBufferSourcePtr> ConstHostBufferSources;

typedef std::vector<HostBufferSourcePtr> HostBufferSources;


//------------------For prototype (signature) of kernel------------------//

typedef std::vector<int> OutputParamIndices;

struct KernelPrototype {
    KernelPrototype(std::string kernelName, const OutputParamIndices& outputParamIndices) {
        m_kernelName = kernelName;
        m_outputParamIndices = outputParamIndices;
    };
    std::string m_kernelName;
    OutputParamIndices m_outputParamIndices;
};

typedef KernelPrototype* KernelPrototypePtr;

typedef std::map<std::string, KernelPrototypePtr> ProgramPrototype;

#endif /* common_h */
