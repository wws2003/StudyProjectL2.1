//
//  main.cpp
//  BitonicSortOpenCL
//
//  Created by wws2003 on 2/11/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <random>
#include <ctime>
#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>
#include "SortingNetworkBasedIntBitonicSorter.h"
#include "IntSortingNetworkCPUImpl.h"
#include "IntSortingNetworkGPUImpl.h"
#include "IntBitonicGPUSorter.h"
#include "SimpleCLProgramFactoryImpl.h"
#include "SimpleCLExecutorImpl.h"
#include "SimpleCLExecutorFactoryImpl.h"
#include "IntHybridBitonicBlockSolver.h"
#include "IntSimpleBitonicBlockBasedSorter.h"
#include "IntGPUBitonicVerticalArraySolver.h"
#include "IntHybridBitonicVerticalArraySolver.h"
#include "IntVerticalArrayBasedBitonicSorter.h"
#include "Util.h"

using namespace std;

#define DEBUG_MODE 0

#define SMALL_SIZE_TO_DEBUG 0

#if SMALL_SIZE_TO_DEBUG

#define ARRAY_SIZE (16)
#define GLOBAL_SIZE_0 (4)
#define GLOBAL_SIZE_1 (4)
#define LOCAL_SIZE (2)
#define LOCAL_SIZE_0 (2)
#define LOCAL_SIZE_1 (2)
#define LOCAL_SIZE_FOR_LOCAL_SORT (256)

#else

#define ARRAY_SIZE (262144) // 512 * 512 = 262144
#define GLOBAL_SIZE_0 (512)
#define GLOBAL_SIZE_1 (512)
#define LOCAL_SIZE (16)
#define LOCAL_SIZE_0 (16)
#define LOCAL_SIZE_1 (32)
#define LOCAL_SIZE_FOR_LOCAL_SORT (512)

#endif

/**
 * Initialize the test data
 * @param entries
 */
void initTestData(ElementList<int>& entries);

/**
 * Test bitonic sort on CPUs
 * @param entries
 */
void testBitonicSortCPU(const ElementList<int>& entries);

/**
 * Test bitonic sort on GPUs using sorting network
 * @param entries
 */
void testSortingNetworkBasedBitonicSortGPU(const ElementList<int>& entries);

/**
 * Test bitonic sort on GPUs without using sorting network
 * @param entries
 */
void testBitonicSortGPU(const ElementList<int>& entries);

/**
 * Test bitonic sorting network
 * @param entries
 * @param pSortingNetwork
 */
void testBitonicSortingNetwork(const ElementList<int>& entries, SortingNetworkPtr<int> pSortingNetwork);

/**
 * Test bitonic sorter
 * @param entries
 */
void testSorter(ISorter<int>* pSorter, const ElementList<int>& entries);

/**
 * Print timespec info
 */
void printTimeSpec(std::string prefixMessage, TimeSpec& timeSpec);

int main() {
    std::srand((unsigned int)std::time(0)); // use current time as seed for random generator
    ElementList<int> entries;
    
    // CPU test
    initTestData(entries);
    std::cout << "Bitonic sorting on CPU using sorting network:\n";
    testBitonicSortCPU(entries);
    
    // GPU test (using sorting network)
    initTestData(entries);
    std::cout << "Bitonic sorting on GPU using sorting network:\n";
    testSortingNetworkBasedBitonicSortGPU(entries);
    
    // GPU test (do not use sorting network)
    initTestData(entries);
    std::cout << "Bitonic sorting on GPU without sorting network:\n";
    testBitonicSortGPU(entries);

    std::cout << "Finish testing bitonic sort\n";
    return 0;
}

void initTestData(ElementList<int>& entries) {
    entries.clear();
    for (int i = 0; i < ARRAY_SIZE; i++) {
        entries.push_back(i);
    }
    std::random_device rd;
    std::mt19937 g(rd());
    
    std::shuffle(entries.begin(), entries.end(), g);
    
#if DEBUG_MODE
    std::cout << "Test dataset:\n";
    for(size_t i = 0; i < ARRAY_SIZE; i++) {
        std::cout << "Test entry [" << i << "]: " << entries[i] << "\n";
    }
#endif

}

void testBitonicSortCPU(const ElementList<int>& entries) {
    // Create CPU sorting network
    SortingNetworkPtr<int> pSortingNetwork = SortingNetworkPtr<int>(new IntSortingNetworkCPUImpl());
    
    // Test
    testBitonicSortingNetwork(entries, pSortingNetwork);
    
    // Release
    freePtr(pSortingNetwork);
}

void testSortingNetworkBasedBitonicSortGPU(const ElementList<int>& entries) {
    // Create GPU sorting network
    SimpleCLProgramFactoryPtr pProgramFactory = SimpleCLProgramFactoryPtr(new SimpleCLProgramFactoryImpl());
    SimpleCLExecutorFactoryPtr pSimpleExecutorFactory = SimpleCLExecutorFactoryPtr(new SimpleCLExecutorFactoryImpl(pProgramFactory));
    
    WorkDims workDims({GLOBAL_SIZE_0, GLOBAL_SIZE_1}, {LOCAL_SIZE, LOCAL_SIZE});
    cl_device_type deviceType = CL_DEVICE_TYPE_GPU;
    SortingNetworkPtr<int> pSortingNetwork = SortingNetworkPtr<int>(new IntSortingNetworkGPUImpl(pSimpleExecutorFactory, workDims, deviceType));

    // Test
    testBitonicSortingNetwork(entries, pSortingNetwork);
    
    // Release
    freePtr(pSortingNetwork);
    freePtr(pSimpleExecutorFactory);
    freePtr(pProgramFactory);
}

void testBitonicSortGPU(const ElementList<int>& entries) {
    // Create GPU bitonic sorter
    SimpleCLProgramFactoryPtr pProgramFactory = SimpleCLProgramFactoryPtr(new SimpleCLProgramFactoryImpl());
    SimpleCLExecutorFactoryPtr pSimpleExecutorFactory = SimpleCLExecutorFactoryPtr(new SimpleCLExecutorFactoryImpl(pProgramFactory));
    WorkDims workDims({GLOBAL_SIZE_0, GLOBAL_SIZE_1}, {LOCAL_SIZE_0, LOCAL_SIZE_1});
    
    BitonicVerticalArraySolverPtr<int> pGPUSolver(new IntGPUBitonicVerticalArraySolver(pSimpleExecutorFactory,
                                                                                       workDims));
    
    BitonicVerticalArraySolverPtr<int> pSolver(new IntHybridBitonicVerticalArraySolver(LOCAL_SIZE_0 * LOCAL_SIZE_1, pGPUSolver));
    
    IntVerticalArrayBasedBitonicSorter sorter(pSolver);
    
    // Test the sorter
    testSorter(&sorter, entries);
    
    // Release
    freePtr(pSolver);
    freePtr(pGPUSolver);
    freePtr(pSimpleExecutorFactory);
    freePtr(pProgramFactory);
}

void testBitonicSortingNetwork(const ElementList<int>& entries,
                               SortingNetworkPtr<int> pSortingNetwork) {
    SortingNetworkBasedBitonicSorter<int> bitonicSorter(pSortingNetwork);
    testSorter(&bitonicSorter, entries);
}

void testSorter(ISorter<int>* pSorter, const ElementList<int>& entries) {
    ElementList<int> sortedEntries;
    
    TimeSpec timeSpec;
    {
        ScopeTimer scopeTimer(&timeSpec);
        pSorter->sort(entries, sortedEntries, SortOrder::ASC);
    }
    printTimeSpec("Sorting time", timeSpec);
    
    size_t elementCnt = sortedEntries.size();
    
    assert(elementCnt == ARRAY_SIZE);
    
    for(unsigned int i = 0; i < elementCnt - 1; i++) {
#if DEBUG_MODE
        std::cout << "Sorted entry [" << i << "]: " << sortedEntries[i] << "\n";
        if (i == elementCnt - 2) {
            std::cout << "Sorted entry [" << (i + 1) << "]: " << sortedEntries[i + 1] << "\n";
        }
#else
        assert(sortedEntries[i] < sortedEntries[i + 1]);
#endif
    }
}

void printTimeSpec(std::string prefixMessage, TimeSpec& timeSpec) {
    std::cout << prefixMessage  << ":\n----" << timeSpec.tv_sec << " s and " << timeSpec.tv_nsec << " ns\n";
}
