uniform float u_deg;
attribute vec4 a_position;

#define LINER_TRANSFORMATION 0
#define SINE_TEST 0
#define ROTATION 0
#define SCALE_ROTATE 0
#define ORTHOGRAPHIC_PROJECTION 0
#define PERSPECTIVE_PROJECTION 0
#define FRUSTUM 1
mat4 view_frustum(
        float angle_of_view,
        float aspect_ratio,
        float z_near,
        float z_far
        ) {
    return mat4(
                vec4(1.0/tan(angle_of_view),           0.0, 0.0, 0.0),
                vec4(0.0, aspect_ratio/tan(angle_of_view),  0.0, 0.0),
                vec4(0.0, 0.0,    (z_far+z_near)/(z_far-z_near), 1.0),
                vec4(0.0, 0.0, -2.0*z_far*z_near/(z_far-z_near), 0.0)
                );
}

mat4 scale(float x, float y, float z)
{
    return mat4(
                vec4(x,   0.0, 0.0, 0.0),
                vec4(0.0, y,   0.0, 0.0),
                vec4(0.0, 0.0, z,   0.0),
                vec4(0.0, 0.0, 0.0, 1.0)
                );
}

mat4 translate(float x, float y, float z)
{
    return mat4(
                vec4(1.0, 0.0, 0.0, 0.0),
                vec4(0.0, 1.0, 0.0, 0.0),
                vec4(0.0, 0.0, 1.0, 0.0),
                vec4(x,   y,   z,   1.0)
                );
}

mat4 rotate_x(float theta)
{
    //float theta = 45.0;
    return mat4(
                vec4(1.0,         0.0,         0.0, 0.0),
                vec4(0.0,  cos(theta),  sin(theta), 0.0),
                vec4(0.0, -sin(theta),  cos(theta), 0.0),
                vec4(0.0,         0.0,         0.0, 1.0)
                );
}

void main()
{
#if LINER_TRANSFORMATION
    mat3 linearTransformation = mat3(vec3(0.5, -0.5, 0.0),
                                     vec3(0.5,  1.0, 0.0),
                                     vec3(0.0,  0.0, 0.0)
                                     );
    gl_Position = vec4(linearTransformation * a_position.xyz, 1.0);
#elif SINE_TEST
    gl_Position   =   a_position;
    gl_Position.x = (a_position.x)+(sin(u_deg));
    gl_Position.y = (a_position.y)+(sin(u_deg));
    gl_Position.z = (a_position.z)+(sin(u_deg));
#elif ROTATION
    float theta = 90.0;
    mat3 rotationMatrix = mat3(vec3(cos(theta), -sin(theta), 0),
                               vec3(cos(theta),  sin(theta), 0),
                               vec3(cos(theta),  sin(theta), 0)
                               );
    gl_Position = vec4(rotationMatrix * a_position.xyz, 2.0);
#elif SCALE_ROTATE
    float theta = 45.0;
    mat3 windowScaleMatrix = mat3( vec3(3.0/4.0, 0.0, 0.0),
                                   vec3(    0.0, 1.0, 0.0),
                                   vec3(    0.0, 0.0, 1.0)
                                   );
    mat3 objScaleMatrix = mat3( vec3(4.0/3.0, 0.0, 0.0),
                                vec3(0.0,     1.0, 0.0),
                                vec3(0.0,     0.0, 1.0)
                                );
    mat3 rotationMatrix = mat3(vec3(cos(theta), -sin(theta), 0),
                               vec3(cos(theta),  sin(theta), 0),
                               vec3(cos(theta),  sin(theta), 0)
                               );
    gl_Position = vec4(windowScaleMatrix * rotationMatrix *  objScaleMatrix * a_position.xyz, 1.0);
#elif ORTHOGRAPHIC_PROJECTION
    float theta = 45.0;
    const mat3 projection = mat3(
                vec3(3.0/4.0, 0.0, 0.0),
                vec3(    0.0, 1.0, 0.0),
                vec3(    0.0, 0.0, 1.0)
                );

    mat3 rotation = mat3(
                vec3(1.0,         0.0,         0.0),
                vec3(0.0,  cos(theta),  sin(theta)),
                vec3(0.0, -sin(theta),  cos(theta))
                );
    mat3 scale = mat3(
                vec3(4.0/3.0, 0.0, 0.0),
                vec3(    0.0, 1.0, 0.0),
                vec3(    0.0, 0.0, 1.0)
                );
    gl_Position = vec4(projection * rotation * scale * a_position.xyz, 1.0);
#elif PERSPECTIVE_PROJECTION
    float theta = 45.0;
    const mat3 projection = mat3(
                vec3(3.0/4.0, 0.0, 0.0),
                vec3(    0.0, 1.0, 0.0),
                vec3(    0.0, 0.0, 0.5)
                );

    mat3 rotation = mat3(
                vec3(1.0,         0.0,         0.0),
                vec3(0.0,  cos(theta),  sin(theta)),
                vec3(0.0, -sin(theta),  cos(theta))
                );
    mat3 scale = mat3(
                vec3(4.0/3.0, 0.0, 0.0),
                vec3(    0.0, 1.0, 0.0),
                vec3(    0.0, 0.0, 1.0)
                );

    vec3 projected_position = projection * rotation * scale * a_position.xyz;
    float perspective_factor = projected_position.z * 0.5 + 1.0;

    gl_Position = vec4(projected_position/perspective_factor, 1.0);
#elseif FRUSTUM
    float theta = 45.0;
    gl_Position = view_frustum(radians(45.0), 4.0/3.0, 0.5, 5.0)
            * translate(cos(theta), 0.0, 3.0+sin(theta))
            * rotate_x(theta)
            * scale(4.0/3.0, 1.0, 1.0)
            * a_position;
#else
    gl_Position   =   a_position;
#endif
}


