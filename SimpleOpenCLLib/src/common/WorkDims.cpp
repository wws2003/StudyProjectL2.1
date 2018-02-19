//
//  WorkDims.cpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/8/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#include "WorkDims.h"
#include <cstring>


WorkDims::WorkDims(const WorkDims& workDims) {
    m_nbDimesions = workDims.m_nbDimesions;
    m_globalSizes = makeCopy(m_nbDimesions, workDims.m_globalSizes);
    m_localSizes = makeCopy(m_nbDimesions, workDims.m_localSizes);
    m_pGlobalOffset = makeCopy(m_nbDimesions, workDims.m_pGlobalOffset);
}

WorkDims::WorkDims(const std::vector<size_t>& globalSizes, const std::vector<size_t>& localSizes, size_t* pGlobalOffset) {
    m_nbDimesions = (cl_uint) globalSizes.size();
    m_globalSizes = makeCopy(globalSizes);
    m_localSizes = makeCopy(localSizes);
    m_pGlobalOffset = makeCopy(m_nbDimesions, pGlobalOffset);
}

WorkDims::WorkDims(cl_uint nbDimesions, size_t* pGlobalSizes, size_t* pLocalSizes, size_t* pGlobalOffset) {
    m_nbDimesions = nbDimesions;
    m_globalSizes = makeCopy(m_nbDimesions, pGlobalSizes);
    m_localSizes = makeCopy(m_nbDimesions, pLocalSizes);
    m_pGlobalOffset = makeCopy(m_nbDimesions, pGlobalOffset);
}

//Shortcut for one-dimensional WorkDims
WorkDims::WorkDims(size_t globalSize, size_t localSize, size_t* pGlobalOffset) {
    m_nbDimesions = 1;
    size_t globalSizes[] = {globalSize};
    size_t localSizes[] = {localSize};
    m_globalSizes = makeCopy(m_nbDimesions, globalSizes);
    m_localSizes = makeCopy(m_nbDimesions, localSizes);
    m_pGlobalOffset = makeCopy(1, pGlobalOffset);
}

WorkDims::~WorkDims() {
    destroy(m_pGlobalOffset);
}

size_t* WorkDims::getCopiedGlobalSizes() const {
    return makeCopy(m_nbDimesions, m_globalSizes);
}

size_t* WorkDims::getCopiedLocalSizes() const {
    return makeCopy(m_nbDimesions, m_localSizes);
}

size_t* WorkDims::getCopiedGlobalOffset() const {
    return makeCopy(m_nbDimesions, m_pGlobalOffset);
}

void WorkDims::copy(size_t* dest, const size_t *pSources, cl_uint nbDimesions) {
    std::memcpy(dest, pSources, nbDimesions * sizeof(size_t));
}

size_t * WorkDims::makeCopy(cl_uint nbDimesions, size_t* sizes) const {
    if (sizes == NULL) {
        return NULL;
    }
    size_t* pSize = new size_t[nbDimesions];
    std::memcpy(pSize, sizes, nbDimesions * sizeof(size_t));
    return pSize;
}

size_t* WorkDims::makeCopy(const std::vector<size_t>& sizes) const {
    size_t nbDims = sizes.size();
    size_t* pSize = new size_t[nbDims];
    for (unsigned int i = 0; i < nbDims; i++) {
        pSize[i] = sizes[i];
    }
    return pSize;
}

void WorkDims::destroy(size_t* sizes) {
    if (sizes != NULL) {
        delete[] sizes;
    }
}

