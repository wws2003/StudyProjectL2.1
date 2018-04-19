//
//  GenericBuffer.hpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/6/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef GenericBuffer_hpp
#define GenericBuffer_hpp

#include "common.h"

template<typename T>
class GenericBuffer : public IHostBufferSource {
public:
    
    /**
     * Construct buffer for one value (short cut for constructing from array)
     * @Param val The value set to buffer
     */
    GenericBuffer(const T& val) {
        m_array = new T[1];
        m_array[0] = val;
        m_size = 1;
        m_autoRelease = true;
    }
    
    /**
     * Construct buffer from array (using the given array memory itself instead of creating new one)
     * @Param array The array set to buffer
     * @Param size Size of the array set to buffer
     */
    GenericBuffer(T* array, size_t size) {
        m_size = size;
        m_array = array;
        m_autoRelease = false;
    }
    
    
    /**
     * Construct buffer from array by copying values into internal memory
     * @Param array The array set to buffer
     * @Param size Size of the array set to copy
     * @Param autoRelease Flag to decide if internal memory should be released in destructor
     */
    GenericBuffer(T* array, size_t size, bool autoRelease) {
        m_size = size;
        if (array != NULL) {
            m_array = new T[size];
            for (unsigned int i = 0; i < m_size; i++) {
                m_array[i] = array[i];
            }
        }
        else {
            m_array = NULL;
        }
        m_autoRelease = autoRelease;
    }
    
    /**
     * Construct buffer by creating array of given size
     * @Param size Size of the array set to buffer
     * @Param autoRelease Flag to decide if internal memory should be released in destructor
     */
    GenericBuffer(size_t size, bool autoRelease) {
        m_array = new T[size];
        m_size = size;
        m_autoRelease = autoRelease;
    }
    
    GenericBuffer(const std::vector<T>& elements, bool autoRelease) {
        size_t size = elements.size();
        m_size = size;
        m_array = new T[size];
        for (size_t i = 0; i < m_size; i++) {
            m_array[i] = elements[i];
        }
        m_autoRelease = autoRelease;
    }
    
    virtual ~GenericBuffer() {
        if (m_autoRelease && m_array != NULL) {
            delete[] m_array;
        }
    };
    
    inline size_t size() const {
        return m_size;
    };
    
    inline const T& operator[](size_t idx) const {
        return m_array[idx];
    };
    
    inline void set(size_t idx, const T& value) {
        m_array[idx] = value;
    }
    
    void release() {
        delete[] m_array;
        m_array = NULL;
    }
    
    //Implement
    void toHostInputBuffer(ParamType paramType, HostBufferExt& inputBuffer, bool toAllocateMem = false) const {
        inputBuffer.m_paramType = paramType;
        inputBuffer.m_arraySize = m_size;
        
        inputBuffer.m_data = cloneArray(toAllocateMem);
        
        inputBuffer.m_elementSize = sizeof(T);
    }
    
    //Implement
    void toHostOutputBuffer(HostBuffer& outputBuffer) const {
        outputBuffer.m_data = m_array;
        outputBuffer.m_arraySize = m_size;
        outputBuffer.m_elementSize = sizeof(T);
    }
    
    //Implement
    void loadFromHostOutputBuffer(const HostBuffer& outputBuffer) {
        for (unsigned int i = 0; i < m_size; i++) {
            T e = ((T*)(outputBuffer.m_data))[i];
            m_array[i] = e;
        }
    }
    
private:
    void* cloneArray(bool toAllocateMem) const {
        if (!toAllocateMem) {
            return m_array;
        }
        else {
            T* clone = new T[m_size];
            for (unsigned int i = 0; i < m_size; i++) {
                clone[i] = m_array[i];
            }
            return clone;
        }
    }
    
    T* m_array;
    size_t m_size;
    bool m_autoRelease;
};

template<typename T>
using GenericBufferPtr = GenericBuffer<T>*;

template<typename T>
using ConstGenericBufferPtr = const GenericBuffer<T>*;

template<typename T>
using ConstGenericBuffers = std::vector<ConstGenericBufferPtr<T> >;

template<typename T>
using GenericBuffers = std::vector<GenericBufferPtr<T> >;

typedef GenericBuffer<float> FloatBuffer;
typedef GenericBuffer<int> IntBuffer;

#endif /* GenericBuffer_hpp */
