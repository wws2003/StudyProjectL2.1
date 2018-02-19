__kernel void addition_kernel(__global const float* a,
                           __global const float* b,
                           __global float* result) {
    
    //Meaning of literal 0 ?: First dimension (in this case is also the only dimension)
    int gid = get_global_id(0);
    
    //Try to access by local id and group id
    
    //This may or may not work, depends on OpenCL implementation ?
    //int gid = get_group_id(0) * get_local_size(0) + get_local_id(0);
    
    result[gid] = a[gid] + b[gid];
}
