#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 OurColor;
out vec3 OutPos;
out vec2 TexCoord;

uniform mat4 Transform;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
	gl_Position = Projection * View * Model * vec4(aPos, 1.f);
	OurColor = aColor;
	OutPos = aPos;
	TexCoord = aTexCoord;
};