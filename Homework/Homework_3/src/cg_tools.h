#ifndef CG_TOOLS
#define CG_TOOLS

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <functional>
using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
class CgTools {
private:
	// settings
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
	int shaderProgram;
	
	const char* glsl_version;
	unsigned long GetFileLength(ifstream& file)
	{
		if (!file.good()) return 0;

		unsigned long pos = file.tellg();
		file.seekg(0, ios::end);
		unsigned long len = file.tellg();
		file.seekg(ios::beg);

		return len;
	}
	int ImportShaderFile(const char* filename, GLchar* &ShaderSource)
	{
		ifstream file;
		file.open(filename, ios::in); // opens as ASCII!
		if (!file) return -1;

		int len = GetFileLength(file);

		if (len == 0) return -2;   // Error: Empty File 

		ShaderSource = new char[len + 1];
		if (*ShaderSource == 0) return -3;   // can't reserve memory

											 // len isn't always strlen cause some characters are stripped in ascii read...
											 // it is important to 0-terminate the real length later, len is just max possible value... 
		ShaderSource[len] = 0;

		unsigned int i = 0;
		while (file.good())
		{
			ShaderSource[i] = file.get();       // get character from file.
			if (!file.eof())
				i++;
		}

		ShaderSource[i] = 0;  // 0-terminate it at the correct position

		file.close();

		return 0; // No Error
	}
	void UnloadShader(GLchar* &ShaderSource) {
		if (ShaderSource != 0)
			delete[] ShaderSource;
		ShaderSource = 0;
	}

	void processInput(GLFWwindow *window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

public:
	GLFWwindow * window;
	CgTools() {}
	~CgTools() {
		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwTerminate();
	}
	int Init () {
		// glfw: initialize and configure
		// ------------------------------
		glfwInit();
		glsl_version = "#version 330";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// glfw window creation
		// --------------------														 
		window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		return 0;

	}
	int BuildShaderProgram(const char* vertex_shader_file, const char* fragment_shader_file) {
		// build and compile our shader program
		// ------------------------------------
		char *vertexShaderSource = NULL;
		char *fragmentShaderSource = NULL;
		ImportShaderFile(vertex_shader_file, vertexShaderSource);
		ImportShaderFile(fragment_shader_file, fragmentShaderSource);

		// vertex shader
		int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		// check for shader compile errors
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			assert(0);
		}

		// fragment shader
		int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		// check for shader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			assert(0);
		}
		// link shaders
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "Error: Shader Program Link FAILED\n" << infoLog << std::endl;
			assert(0);
		}

		UnloadShader(vertexShaderSource);
		UnloadShader(fragmentShaderSource);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return shaderProgram;
	}

	void Start(std::function<void()> fun) {
		// Main loop
		while (!glfwWindowShouldClose(window))
		{
			// Poll and handle events (inputs, window resize, etc.)
			// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
			// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
			// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
			// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
			glfwPollEvents();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::SetNextWindowSize(ImVec2(0, 0));

			fun();

			glfwMakeContextCurrent(window);
			glfwSwapBuffers(window);
		}
	}
};

#endif // !CG_TOOLS
