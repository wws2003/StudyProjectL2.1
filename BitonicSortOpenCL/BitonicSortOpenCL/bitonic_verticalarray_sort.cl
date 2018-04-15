#define ASCENDING (0)
#define DESCENDING (1)

#define DEVICE_REVERSED_ORDER(a) (a == ASCENDING ? DESCENDING : ASCENDING)

#define DEVICE_SWAP(a,b) {int aux = a; a = b; b = aux;}

#define DEVICE_COMPARE_AND_SWAP(a,b,order) {if((a > b && order == ASCENDING) || (b > a && order == DESCENDING)){int aux = a; a = b; b = aux;}}

__kernel void bitonic_vertical_array_solve(const int sortOrder,
                                           const int orderKeptBlockSize,
                                           const int sortBufferSize,
                                           const int sortingDepth,
                                           local int* sortBuffer,
                                           global int* dataBuffer) {
    size_t lI = get_local_id(0);
    size_t lJ = get_local_id(1);
    size_t lDim = get_local_size(0);
    size_t lIndex = lJ * lDim + lI;
    
    size_t groupIdI = get_group_id(0);
    size_t groupIdJ = get_group_id(1);
    size_t groupSize = get_local_size(0) * get_local_size(1);
    size_t offset = groupSize * (groupIdJ * get_num_groups(0) + groupIdI);
    size_t gIndex = offset + lIndex;
    
    // Copy to local
    sortBuffer[lIndex] = dataBuffer[gIndex];
    barrier(CLK_LOCAL_MEM_FENCE);
    
    // Solve local block
    int reversedOrder = DEVICE_REVERSED_ORDER(sortOrder);
    int lSortOrder = ((gIndex / orderKeptBlockSize) % 2 == 0) ? sortOrder : reversedOrder;
    int swapDistance = sortBufferSize >> 1;
    int lowerIndex = lIndex;
    
    int c = sortingDepth;
    while(c > 0) {
        bool isSwapTarget = ((lowerIndex / swapDistance) % 2) == 0;
        if (isSwapTarget) {
            int higherIndex = lowerIndex + swapDistance;
            DEVICE_COMPARE_AND_SWAP(sortBuffer[lowerIndex], sortBuffer[higherIndex], lSortOrder);
        }
        barrier(CLK_LOCAL_MEM_FENCE);
        // Next column
        c--;
        swapDistance >>= 1;
    }
    
    // Write back to global buffer
    dataBuffer[gIndex] = sortBuffer[lIndex];
}