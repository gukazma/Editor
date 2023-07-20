#version 330 core

layout(location = 0) in vec3 a_position;

uniform mat4 mvp_matrix;
uniform vec3 changePos;

void main()
{
    gl_Position = mvp_matrix * vec4(a_position + changePos, 1.0);
}