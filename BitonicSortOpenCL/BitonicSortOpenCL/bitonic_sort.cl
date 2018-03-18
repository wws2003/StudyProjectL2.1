#define ORDER(a,b) { bool swap = reverse ^ (a<b); \
int auxa = a; int auxb = b; a = (swap)?auxb:auxa; b = (swap)?auxa:auxb; }

// #REF: http://www.bealto.com/gpu-sorting_parallel-bitonic-2.html

/**
 @param inc  Largest distance of swap against the data (naturally power of 2)
 @param dir  Initial direction
 @param data Data buffer to sort
 */
__kernel void bitonic_sort_global1(const int inc, const int dir, global int* data) {
    // Calculate index
    size_t i = get_global_id(0);
    size_t j = get_global_id(1);
    size_t dim = get_global_size(0);
    size_t t = j * dim + i;
    
    int swapDistance = inc;
    
    for(; swapDistance > 0; swapDistance >>= 1) {
        // Detect reverse
        int low = t & (inc - 1); // lower order bit, below inc
        int i = (t << 1) - low; // insert 0 at position inc ?
        bool reverse = ((dir & i) == 0);
        
        barrier(CLK_GLOBAL_MEM_FENCE);
        
        // Load
        int x0 = data[i];
        int x1 = data[i + swapDistance];
        
        // Sort
        ORDER(x0, x1);
        barrier(CLK_GLOBAL_MEM_FENCE);
        
        // Store
        data[i] = x0;
        data[i + swapDistance] = x1;
    }
}

/**
 @param inc  Largest distance of swap against the data (naturally power of 2)
 @param dir  Initial direction
 @param data Data buffer to sort
 */
__kernel void bitonic_sort_global_naive(const int inc, const int dir, global int* data) {
    // Calculate index
    size_t i = get_global_id(0);
    size_t j = get_global_id(1);
    size_t dim = get_global_size(0);
    size_t index1 = j * dim + i;
    
    for(int swapDistance = inc; swapDistance > 0; swapDistance >>= 1) {
        // Detect if index1 is a proper lower index
        if ((index1 / swapDistance) % 2) {
            continue;
        }
        
        // Detect reverse
        bool isAscendingOrder = ((index1 / (2 * inc)) % 2 == 0) ? dir : !dir;
        // barrier(CLK_GLOBAL_MEM_FENCE);
        
        // Load and compare (what really can be parallelized)
        size_t index2 = index1 + swapDistance;
        int x1 = data[index1];
        int x2 = data[index2];
        
        bool toSwap = (isAscendingOrder && x1 > x2) || (!isAscendingOrder && x1 < x2);
        if (!toSwap) {
            continue;
        }
        
        // Store (after swapped)
        data[index1] = x2;
        data[index2] = x1;
        
        // How to make sure do not go to next iteration before other threads finished ??
        // barrier only works for work items in the same work group
        barrier(CLK_GLOBAL_MEM_FENCE);
    }
}