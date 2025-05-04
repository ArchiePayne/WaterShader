#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

float vertices[] =
{
	-1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	-1.0f,  1.f, 0.0f,
	 1.0f,  1.f, 0.0f
};

unsigned int indices[] =
{
	0, 1, 2,
	1, 2, 3
};

static unsigned int WindowWidth = 1920;
static unsigned int WindowHeight = 1080;

GLuint compileShader(GLenum shaderType, const std::string& source)
{
	GLuint shader = glCreateShader(shaderType);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "Shader compile error: " << infoLog << std::endl;
	}
	return shader;
}

// Function to create a shader program
GLuint createShaderProgram(const std::string& vertexSource, const std::string& fragmentSource)
{
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cerr << "Program linking error: " << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

std::string LoadShaderFromFile(const std::string& filepath)
{
	std::ifstream fileStream(filepath);
	if (!fileStream.is_open()) {
		std::cerr << "Failed to open shader file: " << filepath << std::endl;
		return "";
	}

	std::stringstream buffer;
	buffer << fileStream.rdbuf();
	return buffer.str();
}

int main() {

	if (!glfwInit()) 
	{
		std::cerr << "GLFW initialization failed!" << std::endl;
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, "Ocean Shader", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Window creation failed!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "GLAD initialization failed!" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	std::string vertexShaderSource = LoadShaderFromFile("shaders/vert.glsl");
	std::string fragmentShaderSource = LoadShaderFromFile("shaders/frag.glsl");

	GLuint shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

	glUseProgram(shaderProgram);

	int uniform_WindowSize = glGetUniformLocation(shaderProgram, "WindowSize");

	glUniform2f(uniform_WindowSize, static_cast<float>(WindowWidth), static_cast<float>(WindowHeight));

	int uniform_NormalizedMouse = glGetUniformLocation(shaderProgram, "NormalizedMouse");

	// Main loop
	float time = 0.0f;
	while (!glfwWindowShouldClose(window)) {

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glUniform2f(uniform_NormalizedMouse, static_cast<float>(xpos / WindowWidth), static_cast<float>(ypos / WindowHeight));

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glUniform1f(glGetUniformLocation(shaderProgram, "time"), time);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		time += 0.01f;
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
