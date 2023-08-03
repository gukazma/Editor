#version 460 core
layout (location = 0) in vec3 a_Position; 
layout (location = 1) in vec3 in_Normal; 

layout (location = 0) out vec3 out_Pos; 
layout (location = 1) out vec3 out_Normal; 

uniform mat4 mvp_matrix;

void main()
{
	out_Pos = vec3(vec4(a_Position, 1.0));
    gl_Position = mvp_matrix * vec4(a_Position, 1.0);
	out_Normal = in_Normal;
}