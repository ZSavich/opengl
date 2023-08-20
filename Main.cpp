#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stbImage/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* Window, int Width, int Height);
void processInput(GLFWwindow* Window);

// Constants
const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;

float MIX_MODIFICATOR = 0.2f;

int main()
{
	glfwInit(); // Initialize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Inform GLFW that we want use OpenGL 3.x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Inform GLFW that we want use OpenGL x.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Inform that we want use Core-Profile

	GLFWwindow* Window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, " OpenGL by Savich Eugene", nullptr, nullptr);
	if (!Window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Make Window's context as main context for current thread
	glfwMakeContextCurrent(Window);

	// Assign a callback function on the change window size event
	glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader OurShader("./shader.vs", "./shader.fgs");

	float vertices[] = {
		// Coordinates		// Colors		// Texture Coordinates
		 0.5f,  0.5f, 0.f,	1.f, 0.f, 0.f,	1.f, 1.f,  // Top Right Vertex
		 0.5f, -0.5f, 0.f,	0.f, 1.f, 0.f,	1.f, 0.f,  // Bottom Right Vertex
		-0.5f, -0.5f, 0.f,	0.f, 0.f, 1.f,	0.f, 0.f, // Bottom Left Vertex
		-0.5f,  0.5f, 0.f,  1.f, 1.f, 0.f,  0.f, 1.f // Top Left Vertex
	};

	unsigned int indices[] = {
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//
	// Working with texture - Start
	//

	float texCoords[] = {
		0.f, 0.f, // Bottom Left
		1.f, 0.f, // Bottom Right
		0.f, 1.f // Top Center
	};


	// Texture #1 - Wood
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Set texture parameters for overlay
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Set texture parameters for filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Loading and Generating a texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// Texture #2 - Face
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// Set texture parameters for overlay
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture parameters for filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);

	data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	//
	// Working with texture - Finish
	//


	OurShader.Use();
	glUniform1i(glGetUniformLocation(OurShader.ID, "texture1"), 0);
	OurShader.SetInt("texture2", 1);

	// Rotate and Scale
	//glm::mat4 Trans = glm::mat4(1.f);
	//Trans = glm::rotate(Trans, glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f));
	//Trans = glm::scale(Trans, glm::vec3(0.5f, 0.5f, 0.5f));

	while (!glfwWindowShouldClose(Window)) 
	{
		processInput(Window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		OurShader.SetFloat("transl", MIX_MODIFICATOR);

		// Rotate and Move
		glm::mat4 TransRot = glm::mat4(1.f);
		TransRot = glm::translate(TransRot, glm::vec3(0.5f, -0.5f, 0.f));
		TransRot = glm::rotate(TransRot, (float)glfwGetTime(), glm::vec3(0.f, 0.f, 1.f));

		OurShader.SetMat("Transform", glm::value_ptr(TransRot));

		OurShader.Use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		// Make a copy of the figure
		TransRot = glm::mat4(1.f);
		TransRot = glm::translate(TransRot, glm::vec3(-0.5f, 0.5f, 0.f));
		const float SinScale = sin(glfwGetTime());
		TransRot = glm::scale(TransRot, glm::vec3(SinScale));

		OurShader.SetMat("Transform", glm::value_ptr(TransRot));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(Window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* Window, int Width, int Height)
{
	glViewport(0, 0, Width, Height);
}

void processInput(GLFWwindow* Window)
{
	// Check if the user pressed the ESC key. Returns GLFW_PRESS or GLFW_RELEASE
	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(Window, true);
	}
	else if (glfwGetKey(Window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		MIX_MODIFICATOR += 0.01f;
		if (MIX_MODIFICATOR >= 1.f)
		{
			MIX_MODIFICATOR = 1.f;
		}
	}
	else if (glfwGetKey(Window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		MIX_MODIFICATOR -= 0.01f;
		if (MIX_MODIFICATOR <= 0.f)
		{
			MIX_MODIFICATOR = 0.f;
		}
	}
}