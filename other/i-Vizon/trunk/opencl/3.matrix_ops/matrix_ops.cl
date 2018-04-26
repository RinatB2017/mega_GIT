__kernel void matrix_add(__global int* mat1_data, __global int* mat2_data,
                        __global int* out_data)
{
    int i = get_global_id(0);
    out_data[i] = mat1_data[i] + mat2_data[i];
}
