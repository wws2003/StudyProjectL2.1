__kernel void inner_product_kernel(__global const float* a,
                              __global const float* b,
                              __global float* singleProducts) {
    
    //Meaning of literal 0 ?: First dimension (in this case is also the only dimension)
    int gid = get_global_id(0);
    
    singleProducts[gid] = a[gid] * b[gid];
}

__kernel void localized_inner_product_kernel(__global const float* a,
                                    __global const float* b,
                                    __local float* localProducts,
                                    __global float* gPartialProducts) {
    
    //Meaning of literal 0 ?: First dimension (in this case is also the only dimension)
    int gid = get_global_id(0);
    int lid = get_local_id(0);
    int localSize = get_local_size(0);
    int groupId = get_group_id(0);
    
    float localSumProd = 0;
    int i;
    
    //Calculate elements of localProducts
    localProducts[lid] = a[gid] * b[gid];
    barrier(CLK_LOCAL_MEM_FENCE);
    
    //Sum up for each group
    //This is time-consuming so calculate only once!
    if(lid == 0) {
        for(i = 0; i < localSize; i++) {
            localSumProd = localSumProd + localProducts[i];
        }
        gPartialProducts[groupId] = localSumProd;
    }
}

__kernel void localized_inner_product_kernel2(__global const float* a,
                                             __global const float* b,
                                             __local float* localProducts,
                                             __global float* gPartialProducts) {
    
    //Meaning of literal 0 ?: First dimension (in this case is also the only dimension)
    int gid = get_global_id(0);
    int lid = get_local_id(0);
    int localSize = get_local_size(0);
    int groupId = get_group_id(0);
    
    float localSumProd = 0;
    int i;
    
    //Calculate elements of localProducts
    localProducts[lid] = a[gid] * b[gid];
    barrier(CLK_LOCAL_MEM_FENCE);
    
    //Sum up for each group
    for(i = localSize / 2; i > 0; i >>= 1) {
        if(lid < i) {
            localProducts[lid] += localProducts[lid + i];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    
    if(lid == 0) {
        gPartialProducts[groupId] = localProducts[lid];
    }
}