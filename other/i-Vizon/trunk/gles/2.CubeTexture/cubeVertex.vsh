attribute vec4 a_position;
uniform mat4 u_mvpMatrix;
void main()
{
    gl_Position = u_mvpMatrix * a_position;
}

/*
0.974278	-1.06066	-0.276316	-0.25
0.795495	0.866025	0.676833	0.612372
0.324759	1.06066	-0.828947	-0.75
0	0	0.105263	2
*/
