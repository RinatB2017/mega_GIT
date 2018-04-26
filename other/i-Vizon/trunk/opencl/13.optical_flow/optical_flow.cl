/// @TIPS: Error: Texture/surface reference must be simple name
///         http://stackoverflow.com/questions/11165992/opencl-texture-surface-reference-must-be-simple-name
///         Method 1 (fails):
///         float4 my_value = read_imagef(my_image,CLK_FILTER_NEAREST|CLK_ADDRESS_CLAMP,my_position);
///         Method 2 (works):
///         sampler_t my_sampler = CLK_FILTER_NEAREST|CLK_ADDRESS_CLAMP;
///         float4 my_value = read_imagef(my_image,my_sampler,my_position);

sampler_t sampler = CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

__kernel void optical_flow (read_only image2d_t srcImage1,
                            read_only image2d_t srcImage2,
                            write_only image2d_t dstImage,
                            const float scale,
                            const float offset,
                            const float lambda,
                            const float threshold)
{
    int2 coords = (int2)(get_global_id(0), get_global_id(1));
    float4 a	= read_imagef(srcImage1, sampler, coords);
    float4 b	= read_imagef(srcImage2, sampler, coords);
    int2 x1 	= (int2)(offset, 0.f);
    int2 y1 	= (int2)(0.f, offset);
    //get the difference
    float4 curdif = b - a;

    //calculate the gradient
    //Image 2 first
    float4 gradx = read_imagef(srcImage2, sampler, coords+x1)
            - read_imagef(srcImage2, sampler, coords-x1);
    //Image 1
    gradx += read_imagef(srcImage1, sampler, coords+x1)
            - read_imagef(srcImage1, sampler, coords-x1);
    //Image 2 first
    float4 grady = read_imagef(srcImage2, sampler, coords+y1)
            - read_imagef(srcImage2, sampler, coords-y1);
    //Image 1
    grady += read_imagef(srcImage1, sampler, coords+y1)
            - read_imagef(srcImage1, sampler, coords-y1);

    float4 sqr = (gradx*gradx)+(grady*grady)+(float4)(lambda,lambda, lambda, lambda);
    float4 gradmag = sqrt(sqr);


    float4 vx = curdif*(gradx/gradmag);


    float vxd = vx.x;//assumes greyscale

    //format output for flowrepos, out(-x,+x,-y,+y)
    float2 xout = (float2)(fmax(vxd,0.f),fabs(fmin(vxd,0.f)));
    xout *= scale;

    float4 vy = curdif*(grady/gradmag);
    float vyd = vy.x;//assumes greyscale
    //format output for flowrepos, out(-x,+x,-y,+y)
    float2 yout = (float2)(fmax(vyd,0.f),fabs(fmin(vyd,0.f)));
    yout *= scale;


    float4 out = (float4)(xout, yout);
    float cond = (float)isgreaterequal(length(out), threshold);
    out *= cond;

    write_imagef(dstImage, coords, out);
}
