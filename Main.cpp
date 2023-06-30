#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* Window, int Width, int Height);
void processInput(GLFWwindow* Window);

// Constants
const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;

// Source code of the simple vertex shader
const char* VertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.f);\n"
"}\n";

// Source code of the simle fragment shader
const char* FragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.f, 0.5f, 0.2f, 1.f);\n"
"}\n";

int main()
{
	glfwInit(); // Initialize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Inform GLFW that we want use OpenGL 3.x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Inform GLFW that we want use OpenGL x.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Inform that we want use Core-Profile

	GLFWwindow* Window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, " OpenGL by Savich Eugene", nullptr, nullptr);

	float vertices[] = {
		-0.5f, -0.5f, 0.f,
		0.5f, -0.5f, 0.f,
		0.f, 0.5f, 0.f
	};

	// Create OpenGL Vertex Buffer Object
	unsigned int VBO;
	// Set VBO identificator for the object
	glGenBuffers(1, &VBO);
	// Assign VBO to GL_ARRAY_BUFFER (type of Vertex Buffer object)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy vertex data to the buffer's memory
	// GL_STREAM_DRAW - set data only once and use only once by graphic process
	// GL_STATIC_DRAW - set data only once and use multiple times
	// GL_DYNAMIC_DRAW - set data and use it multiple times
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create and Compile Vertex Shader
	unsigned int VertexShader;
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Set source code of the shared's object and compile it
	glShaderSource(VertexShader, 1, &VertexShaderSource, NULL);
	glCompileShader(VertexShader);
	// Check compilation errors for vertex shader
	int Success;
	char InfoLog[512];
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(VertexShader, 512, NULL, InfoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << InfoLog << std::endl;
	}

	// Create and Compile Fragment Shader
	unsigned int FragmentShader;
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &FragmentShaderSource, NULL);
	glCompileShader(FragmentShader);
	// Check compilation errors for fragment shader
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(FragmentShader, 512, NULL, InfoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILAION_FAILED\n" << InfoLog << std::endl;
	}

	// Create Shader Programm
	unsigned int ShaderProgram;
	ShaderProgram = glCreateProgram();
	// Attach created Shaders to the Shader Program
	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);
	glLinkProgram(ShaderProgram);
	// Check attaching errors
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(ShaderProgram, 512, NULL, InfoLog);
		std::cout << "ERROR::SHADERPROGRAM::LINK_FAILED\n" << InfoLog << std::endl;
	}
	// Activate result of linking shaders to the Shader Program
	glUseProgram(ShaderProgram);

	// Clear shader objects after we attached them to the SP, we don't need them anymore.
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);

	if (!Window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	// Make Window's context as main context for current thread
	glfwMakeContextCurrent(Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

	// Assign a callback function on the change window size event
	glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);

	// Render loop
	while (!glfwWindowShouldClose(Window)) // If the user tried to close the window - return true and finish the loop
	{
		// Process inputs
		processInput(Window);

		// Complete rendering
		glClearColor(0.5f, 0.5f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Spaps color buffer and displays it on the screen
		glfwSwapBuffers(Window);

		// Observe if initialized any events (input key, mouse move, etc.)
		glfwPollEvents();
	}

	// Remove/Clear GLFW resources
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
}