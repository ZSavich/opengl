#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 OurColor;
out vec3 OutPos;
out vec2 TexCoord;

uniform mat4 Transform;

void main()
{
	gl_Position = Transform * vec4(aPos, 1.f);
	OurColor = aColor;
	OutPos = aPos;
	TexCoord = aTexCoord;
};