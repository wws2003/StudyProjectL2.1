/**
 @param swapDistance Index distance from lower element to upper element to be compared
 @param orderKeptSwapCnt Number of comparisons in which comparing order kept
 @param isFirstOrderAscending Order of first comparison ?
 @param dataBuffer Buffer of elements to sort. Acts as inout param
 */
__kernel void bitonic_swap_int1(const int swapDistance,
                                const int orderKeptSwapCnt,
                                const int isFirstOrderAscending,
                                global int* dataBuffer) {
    // Calculate indexes
    size_t i = get_global_id(0);
    size_t j = get_global_id(1);
    size_t dim = get_global_size(0);
    size_t index1 = j * dim + i;
    
    // Check if index1 is actually index of item to be compared and swapped
    if ((index1 / swapDistance) % 2) {
        return;
    }

    // Calculate order type on the fly rather than pre-calculated
    bool isAscendingOrder = ((index1 / (2 * orderKeptSwapCnt)) % 2 == 0) ? isFirstOrderAscending : !isFirstOrderAscending;
    
    // Compare and swap then store result (in-place)
    unsigned int index2 = index1 + swapDistance;
    int val1 = dataBuffer[index1];
    int val2 = dataBuffer[index2];
    int min = val1 < val2 ? val1 : val2;
    int max = val1 >= val2 ? val1 : val2;
    dataBuffer[index1] = isAscendingOrder ? min : max;
    dataBuffer[index2] = isAscendingOrder ? max : min;
}