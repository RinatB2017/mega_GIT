const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
                          CLK_ADDRESS_CLAMP_TO_EDGE   |
                          CLK_FILTER_NEAREST;
__kernel void add_image(read_only image2d_t src1, read_only image2d_t src2,
                        write_only image2d_t dst)
{
        int x = get_global_id(0);
        int y = get_global_id(1);
        int width = get_image_width(src1);
        float4 src1_pixel = read_imagef(src1, sampler, (int2)(x,y));
        float4 src2_pixel = read_imagef(src2, sampler, (int2)(x,y));
        float alpha = 0.5f;

        write_imagef(dst, (int2)(x,y), ((alpha * src1_pixel) + (1-alpha) * src2_pixel));
 }
	

__kernel void imgRW(read_only image2d_t imgA, write_only image2d_t imgB)
{
    int2 coords = (int2)(get_global_id(0), get_global_id(1));
    float4 pixel = read_imagef(imgA, sampler, coords);
    write_imagef(imgB, coords, (float4)(pixel.z, pixel.y, pixel.x, pixel.w));
    //write_imagef(imgB, coords, pixel);
}
