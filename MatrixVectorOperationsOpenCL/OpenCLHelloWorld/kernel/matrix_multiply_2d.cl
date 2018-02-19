__kernel void mmul_simple_2d(const int Ndim,
                             const int Pdim,
                             const int Mdim,
                             global float* A,
                             global float* B,
                             global float* C) {
    
    //Index of the row to be calculated
    
    //A: Ndim * Pdim
    //B: Pdim * Mdim
    //C: Ndim * Mdim
    
    int aRow = get_global_id(0);
    int bCol = get_global_id(1);
    
    int i;
    float tmp = 0.0;
    
    int aPrefixIndex = aRow * Pdim;
    for(i = 0; i < Pdim; i++) {
        tmp += A[aPrefixIndex + i] * B[i * Mdim + bCol];
    }
    
    C[aPrefixIndex + bCol] = tmp;
}

__kernel void mmul_localized_2d1(const int Ndim,
                             const int Pdim,
                             const int Mdim,
                             global float* A,
                             global float* B,
                             local float* localA,
                             global float* C) {
    
    //Index of the row to be calculated
    
    //A: Ndim * Pdim
    //B: Pdim * Mdim
    //C: Ndim * Mdim
    
    int aRow = get_global_id(0);
    int bCol = get_global_id(1);
    
    //This implementation requires localsize[0] must be 1
    int lCol = get_local_id(1);
    int colStep = get_local_size(1);
    
    int i, gCol;
    float tmp = 0.0;
    
    int aPrefixIndex = aRow * Pdim;
    
    //Read to localA
    for(gCol = lCol; gCol < Pdim; gCol += colStep) {
        localA[gCol] = A[aPrefixIndex + gCol];
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    
    for(i = 0; i < Pdim; i++) {
        tmp += localA[i] * B[i * Mdim + bCol];
    }
    
    C[aPrefixIndex + bCol] = tmp;
}

__kernel void mmul_localized_2d2(const int Ndim,
                                 const int Pdim,
                                 const int Mdim,
                                 global float* A,
                                 global float* B,
                                 local float* localARows, //Rows from A, each has size PDim
                                 local float* localBCols, //Columns from B, each has size PDim
                                 global float* C) {
    
    //Index of the row to be calculated
    
    //A: Ndim * Pdim
    //B: Pdim * Mdim
    //C: Ndim * Mdim
    int rowGroupId = get_group_id(0);
    int colGroupId = get_group_id(1);
    
    int lRow = get_local_id(0);
    int rowStep = get_local_size(0);
    
    int lCol = get_local_id(1);
    int colStep = get_local_size(1);
    
    //Read to localARows: localARows = A[gFirstRowA][|] -> A[gFirstRowA + rowStep - 1][|]
    int gFirstRowA = rowStep * rowGroupId;
    
    //Each work item loads a part of row gRowA
    int gRowA = gFirstRowA + lRow;
    
    int lPrefixIndexA = lRow * Pdim;
    for(int gCol = lCol; gCol < Pdim; gCol += colStep) {
        localARows[lPrefixIndexA + gCol] = A[gRowA * Pdim + gCol];
    }
    
    //Read to localBCols: localBCols = B[|][gFirstColumnB] -> B[|][gFirstColumnB + colStep - 1]
    int gFirstColumnB = colStep * colGroupId;
    
    //Each work item loads a part of column gColB
    int gColB = gFirstColumnB + lCol;
    
    int lPrefixIndexB = lCol * Pdim;
    for(int gRow = lRow; gRow < Pdim; gRow += rowStep) {
        localBCols[lPrefixIndexB + gRow] = B[gRow * Pdim + gColB];
    }

    barrier(CLK_LOCAL_MEM_FENCE);
    
    float tmp = 0.0;
    //Calculate inner product of only one row in localARows and one column in localBCols
    for(int i = 0; i < Pdim; i++) {
        tmp += localARows[lPrefixIndexA + i] * localBCols[lPrefixIndexB + i];
        //tmp += A[gRowA * Pdim + i] * B[i * Mdim + gColB];
    }
    
    C[gRowA * Pdim + gColB] = tmp;
}