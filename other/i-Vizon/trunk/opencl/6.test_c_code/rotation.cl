const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
                            CLK_ADDRESS_CLAMP |
                            CLK_FILTER_LINEAR;
__kernel
void img_rotate(__global unsigned char* src_data,
                __global unsigned char* dest_data,
                int  W,
                int  H,
                float  sinTheta,
                float  cosTheta)
{

   //Work-item gets its index within index space
   const int ix = get_global_id(0);
   const int iy = get_global_id(1);

   //Calculate location of data to move into (ix,iy)
   //Output decomposition as mentioned
   float x0 = W/2.0f;
   float y0 = H/2.0f;

   float xOff = ix - x0;
   float yOff = iy - y0;

   int xpos = (int)(xOff*cosTheta + yOff*sinTheta + x0 );
   int ypos = (int)(yOff*cosTheta - xOff*sinTheta + y0 );

   // Bounds Checking
   if((xpos>=0) && (xpos< W) && (ypos>=0) && (ypos< H))
   {

      // Read (ix,iy) src_data and store at (xpos,ypos) in
      // dest_data
      // In this case, because we rotating about the origin
      // and there is no translation, we know that (xpos,ypos)
      // will be unique for each input (ix,iy) and so each
      // work-item can write its results independently

      dest_data[iy*W+ix] = src_data[ypos*W+xpos];
   }
   else
    dest_data[iy*W+ix] = 0;
}

__kernel void imgRW(read_only image2d_t imgA, write_only image2d_t imgB)
{
    int2 coords = (int2)(get_global_id(0), get_global_id(1));
    uint4 inpixel = read_imageui(imgA, sampler, coords);
    uint4 pixel = (uint4) (0,
                            0,
                            0,
                            255);
    write_imageui(imgB, coords, pixel);
}
