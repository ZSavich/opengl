#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
	// Program's Identificator
	unsigned int ID;

	// Constructor that reads data and builds shaders
	Shader(const char* VertexPath, const char* FragmentPath);

	// Using/Activation the Shader
	void Use();

	// Usefull uniform functions
	void SetBool(const std::string& Name, bool Value) const;
	void SetInt(const std::string& Name, int Value) const;
	void SetFloat(const std::string& Name, float Value) const;
	void SetMat(const std::string& Name, const GLfloat* ValuePtr) const;
};

#endif

