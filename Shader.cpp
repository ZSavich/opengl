#include "Shader.h"

Shader::Shader(const char* VertexPath, const char* FragmentPath)
{
	// Step 1. Get source code of the vertex/fragment shader from FilePahth variable
	std::string VertexCode;
	std::string FragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// Make sure that ifstream objects can throw the exeptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// Open files
		vShaderFile.open(VertexPath);
		fShaderFile.open(FragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		// Read file buffers into threads
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// Close files
		vShaderFile.close();
		fShaderFile.close();

		// Convert data from threads into string variables
		VertexCode = vShaderStream.str();
		FragmentCode = fShaderStream.str();
	}

	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = VertexCode.c_str();
	const char* fShaderCode = FragmentCode.c_str();

	// Step 2. Shader Compilation
	unsigned int Vertex, Fragment;
	int Success;
	char InfoLog[512];

	// Vertex Shader
	Vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(Vertex, 1, &vShaderCode, NULL);
	glCompileShader(Vertex);

	// If there are compilation errors, show message about them
	glGetShaderiv(Vertex, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(Vertex, 512, NULL, InfoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << InfoLog << std::endl;
	}

	// Fragment Shader
	Fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(Fragment, 1, &fShaderCode, NULL);
	glCompileShader(Fragment);

	glGetShaderiv(Fragment, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(Fragment, 512, NULL, InfoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << InfoLog << std::endl;
	}

	// Shader Program
	ID = glCreateProgram();
	glAttachShader(ID, Vertex);
	glAttachShader(ID, Fragment);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(ID, 512, NULL, InfoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << InfoLog << std::endl;
	}

	// Clear Shaders after we linked them to the Shader Program
	glDeleteShader(Vertex);
	glDeleteShader(Fragment);
}

void Shader::Use()
{
	glUseProgram(ID);
}

void Shader::SetBool(const std::string& Name, bool Value) const
{
	glUniform1i(glGetUniformLocation(ID, Name.c_str()), (int)Value);
}

void Shader::SetInt(const std::string& Name, int Value) const
{
	glUniform1i(glGetUniformLocation(ID, Name.c_str()), Value);
}

void Shader::SetFloat(const std::string& Name, float Value) const
{
	int OffsetLocation = glGetUniformLocation(ID, Name.c_str());
	glUniform1f(OffsetLocation, Value);
}

void Shader::SetMat(const std::string& Name, const GLfloat* ValuePtr) const
{
	unsigned int TransformLoc = glGetUniformLocation(ID, Name.c_str());
	glUniformMatrix4fv(TransformLoc, 1, GL_FALSE, ValuePtr);
}