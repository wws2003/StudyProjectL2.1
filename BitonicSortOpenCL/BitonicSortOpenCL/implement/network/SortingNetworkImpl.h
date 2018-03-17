//
//  SortingNetworkImpl.h
//  BitonicSortOpenCL
//
//  Created by wws2003 on 2/11/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#ifndef SortingNetworkImpl_h
#define SortingNetworkImpl_h

#include <map>
#include <functional>
#include "SortingNetwork.h"

template<typename V>
using LowerUpperSwapFunc = std::function<void(ElementList<V>&,
                                              size_t,
                                              size_t)>;

template<typename W>
using LowerUpperSwapFuncMap = std::map<SortOrder, LowerUpperSwapFunc<W> >;

typedef std::function<SortOrder (size_t, size_t, SortOrder, SortOrder )> SortOrderDetectFunc;

/*
 * Simple implementation of sorting network
 */
template<typename U>
class SortingNetworkImpl : public SortingNetwork<U> {
public:
    SortingNetworkImpl();
    virtual ~SortingNetworkImpl(){};
    
    /**
     * @Override
     * Set the elements to the internal sorting network
     * @param elements
     */
    virtual void set(const ElementList<U>& elements);
    
    /**
     * @Override
     * Get size of sorting network
     */
    virtual size_t size();
    
    /**
     * @Override
     * Conduct swap though all the network, apply changes into the internal collection
     * @param compareDistance Element[i] will be compared with Element[i + compareDistance]
     * @param sortOrderKeptSwapCnt Number of comparison kept the same order
     * @param firstSortOrder First comparison order
     */
    virtual void swap(int swapDistance,
                      int sortOrderKeptSwapCnt,
                      SortOrder firstSortOrder);
    
    /**
     * Retrieve back elements into output parameter
     * @param elements
     */
    virtual void collect(ElementList<U>& elements) const;
    
private:
    /**
     * Validate size of elements (only accept power of 2)
     * @param elementCnt
     */
    void validateElementsCount(size_t elementCnt) const;
    
    ElementList<U> m_elements;
    
    SortOrderDetectFunc m_sortOrderDetectFunc;
    LowerUpperSwapFuncMap<U> m_lowerUpperSwapFuncMap;
};

#endif /* SortingNetworkImpl_h */
