const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
                          CLK_ADDRESS_CLAMP_TO_EDGE   |
                          CLK_FILTER_NEAREST;
__kernel void convolution(__read_only  image2d_t  sourceImage,
                          __write_only image2d_t  outputImage,
                          int rows,
                          int cols,
                          __constant float* filter,
                          int filterWidth)
{
   // Store each work-item’s unique row and column
   int column = get_global_id(0);
   int row    = get_global_id(1);

   // Half the width of the filter is needed for indexing
   // memory later
   int halfWidth = (int)(filterWidth/2);

   // All accesses to images return data as four-element vector
   // (i.e., float4), although only the 'x' component will contain
   // meaningful data in this code
   float4 sum = {0.0f, 0.0f, 0.0f, 0.0f};

   // Iterator for the filter
   int filterIdx = 0;

   // Each work-item iterates around its local area based on the
   // size of the filter
   int2 coords;  // Coordinates for accessing the image
   // Iterate the filter rows
   for(int i = -halfWidth; i <= halfWidth; i++)
   {
      coords.y = row + i;

      // Iterate over the filter columns
      for(int j = -halfWidth; j <= halfWidth; j++)
      {
         coords.x = column + j;

         float4 pixel;
         // Read a pixel from the image.  A single channel image
         // stores the pixel in the 'x' coordinate of the returned
         // vector.
         pixel = read_imagef(sourceImage, sampler, coords);
         sum.x += pixel.x * filter[filterIdx++];
      }
   }

   // Copy the data to the output image if the
   // work-item is in bounds
   if(row < rows && column < cols)
   {
      coords.x = column;
      coords.y = row;
      write_imagef(outputImage, coords, sum);
   }
}


__kernel void imgRW(read_only image2d_t imgA, write_only image2d_t imgB, sampler_t sampler)
{
    int2 coords = (int2)(get_global_id(0), get_global_id(1));
    float4 pixel = read_imagef(imgA, sampler, coords);
    write_imagef(imgB, coords, (float4)(pixel.z, pixel.y, pixel.x, pixel.w));
    //write_imagef(imgB, coords, pixel);
}
