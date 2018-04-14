#define DEVICE_SWAP(a,b) {int aux = a; a = b; b = aux;}

#define ASCENDING (0)
#define DESCENDING (1)

/**
 *@param itemCount      Number of items in the block to be sorted (must be fit within an workgroup)
 *@param sortDirection  Direction of sort for the entire block
 *@param globalOffset   Offset in the global bufffer
 *@param sortBuffer     Local buffer to manipulate the sort
 *@param outputBuffer   Output of the sort
 */
__kernel void bitonic_sort_local1(const int itemCount,
                                  const int sortDirection,
                                  const int globalOffset,
                                  local int* sortBuffer,
                                  global int* outputBuffer) {
    
    // Any constraint that itemCount must match the local size ?
    size_t localIndex = get_local_id(0);
    
    // Copy to (local) sort buffer (just one for each work item)
    sortBuffer[localIndex] = outputBuffer[globalOffset + localIndex];
    barrier(CLK_LOCAL_MEM_FENCE);
    
    // Do sort
    // 1. Swap following the given sort direction
    // 2. Sort sub blocks until the size of one, with the direction determined by the given sort direction
    int halfItemCount = itemCount >> 1;
    
    for(int swapDistance = itemCount >> 1; swapDistance > 1; swapDistance >>= 1) {
        int lowerIndex = localIndex;
        int higherIndex = lowerIndex + swapDistance;
        bool inRange = higherIndex < itemCount;
        bool toSwap = false;
        
        if (inRange) {
            // Check if need to swap
            int x0 = sortBuffer[lowerIndex];
            int x1 = sortBuffer[higherIndex];
            // To swap: When sortDirection = ASCENDING and x1 < x0 or sortDirection = DESCENDING and x0 > x1 or
            toSwap = (sortDirection == ASCENDING) && (x1 < x0) || (sortDirection == DESCENDING) && (x0 > x1);
        }
        
        // Wait for all work items finish their work
        barrier(CLK_LOCAL_MEM_FENCE);
        
        // Swap if needed
        if (toSwap) {
            DEVICE_SWAP(sortBuffer[lowerIndex], sortBuffer[higherIndex]);
        }
    }
    
    // Finally store to global memory
    outputBuffer[globalOffset + localIndex] = sortBuffer[localIndex];
}