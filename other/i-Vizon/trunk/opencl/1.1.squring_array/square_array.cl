/**
----------------------------------------
|  0    |   1   |   2   |    3  |...    GLOBAL G_X
----------------------------------------
|0|1|2|3|0|1|2|3|0|1|2|3|0|1|2|3|...    LOCAL L_X
----------------------------------------
|   0   |   4   |  8    |....           G_X * SIZE_L_X
----------------------------------------
|0|1|2|3|4|5|6|7|8|9|.........          LX + (G_X * SIZE_L_X)
----------------------------------------

*/

#pragma OPENCL EXTENSION cl_khr_fp64 : enable


__kernel
void cl_square_array_int(__global float *in_data, __global float *out_data)
{
    int g_x = get_global_id(0); // threadIdx
    int l_x = get_local_id(0);  //blockIdx

    int size_g_x = get_global_size(0);
    int size_l_x = get_local_size(0);

    if(g_x < size_g_x && l_x < size_l_x)
        out_data[l_x + (g_x * size_l_x) ] = sqrt(in_data[l_x + (g_x * size_l_x) ]) ;
                                            //in_data[l_x + (g_x * size_l_x) ];
}
