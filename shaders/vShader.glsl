#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
out vec2 fragUV;

layout (location = 2) uniform mat4 mat;

void main()
{
	gl_Position= mat * vec4(position, 1);
	fragUV = uv;
}