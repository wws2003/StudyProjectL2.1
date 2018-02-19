__kernel void mmul_c_row_kernel(const int Ndim,
                                const int Pdim,
                                const int Mdim,
                                global float* A,
                                global float* B,
                                global float* C) {
    
    //Index of the row to be calculated
    
    //A: Ndim * Pdim
    //B: Pdim * Mdim
    //C: Ndim * Mdim
    
    int row = get_global_id(0);
    int i,j;
    float tmp;
    
    if(row < Ndim) {
        //C[row][i] = \sigma_j(A[row][j] * B[j][i])
        for(i = 0; i < Mdim; i++) {
            tmp = 0.0;
            for(j = 0; j < Pdim; j++) {
                tmp += A[row * Pdim + j] * B[j * Mdim + i];
            }
            C[row * Mdim + i] = tmp;
        }
    }
}

__kernel void mmul_c_private_mem_row_kernel(const int Ndim,
                                            const int Pdim,
                                            const int Mdim,
                                            global float* A,
                                            global float* B,
                                            global float* C) {
    
    //A: Ndim * Pdim
    //B: Pdim * Mdim
    //C: Ndim * Mdim
    
    //Index of the row to be calculated
    int row = get_global_id(0);
    int i,j;
    float tmp;
    
    //Row of factor matrix A is temporary stored into private memory to avoid loading from global memory each time C[row][i] is calculated
    
    float Awrk[1000];
    for(i = 0; i < Pdim; i = i + 1) {
        Awrk[i] = A[row * Pdim + i] ;
    }
    
    //C[row][i] = \sigma_j(A[row][j] * B[j][i])
    //2 loops are too time-consuming
    for(i = 0; i < Mdim; i++) {
        tmp = 0.0;
        for(j = 0; j < Pdim; j++) {
            tmp += Awrk[j] * B[j * Mdim + i];
        }
        C[row * Mdim + i] = tmp;
    }
}

#define SEQUENTIAL_LOAD (1)

__kernel void mmul_c_private_bloc_mem_row_kernel(const int Ndim,
                                                 const int Pdim,
                                                 const int Mdim,
                                                 global float* A,
                                                 global float* B,
                                                 local float* Bwrk, 
                                                 global float* C) {
    
    //Param local float* Brwk is needed as dynamic memory allocation is not supported in kernel
    
    //A: Ndim * Pdim
    //B: Pdim * Mdim
    //C: Ndim * Mdim
    
    //Index of the row to be calculated
    int iGlob = get_global_id(0);
    int iLoc = get_local_id(0);
    int nLoc = get_local_size(0);
    
    int i, j, localRow, k;
    float tmp;
    
    //Row of factor matrix A is temporary stored into private memory to avoid loading from global memory each time C[row][i] is calculated
    
    float Awrk[1000];
    for(i = 0; i < Pdim; i = i + 1) {
        Awrk[i] = A[iGlob * Pdim + i] ;
    }
    
    //Loop throghs columns of B matrix
    for(j = 0; j < Mdim; j = j + 1) {
        //MARK: Column j of B
        
        //Load data from global B[] to local Bwrk so that all work items in group can access
        //For each work item, only round(Pdim / nLoc) elements should be loaded
        
#ifdef SEQUENTIAL_LOAD
        //If want to load Bwrk sequentially
        //i.e. Work item 0-locally load elements B[0], B[1], B[2],..., B[nLoc - 1]
        for(localRow = iLoc * nLoc; localRow < Pdim; localRow = localRow + 1) {
            //Row localRow column j element of B
            Bwrk[localRow] = B[localRow * Mdim + j];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
#else
        //If want to load Bwrk sparsely
        //i.e. Work item 0-locally load elements B[0], B[nLoc - 1], B[2 * nLoc - 1], ...
        for(localRow = iLoc; localRow < Pdim; localRow = localRow + nLoc) {
            //Row localRow column j element of B
            Bwrk[localRow] = B[localRow * Mdim + j];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
#endif
        
        //Conduct the dot product
        tmp = 0.0;
        for(k = 0; k < Pdim; k = k + 1) {
            tmp += Awrk[k] * Bwrk[k];
        }
        
        C[iGlob * Mdim + j] = tmp;
    }
}