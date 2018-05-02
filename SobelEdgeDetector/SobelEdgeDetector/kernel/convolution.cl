
__kernel void convolution(__read_only image2d_t sourceImage,
                          __write_only image2d_t outputImage,
                          const int rows,
                          const int cols,
                          __constant float* filter,
                          const int filterWidth,
                          sampler_t sampler) {
    
    // Each work-itme is corresponding to a coordinate (on the image ?)
    int col = get_global_id(0);
    int row = get_global_id(1);
    
    // Half the width of the filter is needed for indexing
    // memory later
    int halfWidth = (int)(filterWidth/2);
    
    // Access to image requires 4-elements vector, though only the first one (.x) is used
    float4 sum = {0.0f, 0.0f, 0.0f, 0.0f};
    
    // Iterator for the filter
    int filterIdx = 0;
    
    // Coordinate for accessing the image (read/write via .x, .y field)
    int2 coords;
    // Iterate the filter rows
    for(int i = -halfWidth; i <= halfWidth; i++) {
        coords.y = row + i;
        // Suppose filter is a square
        for(int j = -halfWidth; j <= halfWidth; j++) {
            coords.x = col + j;
            // Read pixel from image
            float4 pixel;
            pixel = read_imagef(sourceImage, sampler, coords);
            // Update sum
            sum.x += pixel.x * filter[filterIdx++];
        }
    }
    
    // Output if in bounds
    if (col < cols && row < rows) {
        coords.x = col;
        coords.y = row;
        // Write operation
        write_imagef(outputImage, coords, sum);
    }
    
}