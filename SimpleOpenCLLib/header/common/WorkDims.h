//
//  WorkDims.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/8/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef WorkDims_h
#define WorkDims_h

#include "common.h"

class WorkDims {
public:
    WorkDims(const WorkDims& workDims);
    
    WorkDims(const std::vector<size_t>& globalSizes, const std::vector<size_t>& localSizes, size_t* pGlobalOffset = NULL);
    
    WorkDims(cl_uint nbDimesions, size_t* pGlobalSizes, size_t* pLocalSizes, size_t* pGlobalOffset = NULL);
    
    //Shortcut for one-dimensional WorkDims
    WorkDims(size_t globalSize, size_t localSize, size_t* pGlobalOffset = NULL);
    
    virtual ~WorkDims();
    
    inline size_t getLocalSize(cl_uint dimIndex) const {
        return m_localSizes[dimIndex];
    }
    
    inline size_t getGlobalSize(cl_uint dimIndex) const {
        return m_globalSizes[dimIndex];
    }
    
    inline cl_uint getNbDimensions() const {
        return m_nbDimesions;
    }
    
    inline size_t* globalSizes() const {
        return m_globalSizes;
    }
    
    inline size_t* localSizes() const {
        return m_localSizes;
    }
    
    inline size_t* globalOffsets() const {
        return m_pGlobalOffset;
    }
    
    size_t* getCopiedGlobalSizes() const;
    
    size_t* getCopiedLocalSizes() const;
    
    size_t* getCopiedGlobalOffset() const;
    
private:
    void copy(size_t* dest, const size_t *pSources, cl_uint nbDimesions);
    
    size_t* makeCopy(cl_uint nbDimesions, size_t* sizes) const;
   
    size_t* makeCopy(const std::vector<size_t>& sizes) const;
    
    void destroy(size_t* pSize);
    
    cl_uint  m_nbDimesions;
    size_t* m_globalSizes;
    size_t* m_localSizes;
    size_t* m_pGlobalOffset;
};


#endif /* WorkDims_hpp */
