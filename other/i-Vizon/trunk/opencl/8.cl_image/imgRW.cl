const sampler_t sampler = CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

__kernel void magesTest(read_only image2d_t src, write_only image2d_t dst)
{
    int x = get_global_id(0);
    int y = get_global_id(1);

    float4 pixel = read_imagef(src, sampler, (int2)(x,y));
    write_imagef(dst, (int2)(x,y), pixel);
    //write_imagef(dst, (int2)(x,y), (float4)(pixel.z, pixel.y, pixel.x, pixel.w));
}
