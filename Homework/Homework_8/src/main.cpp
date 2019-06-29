#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include "curve.h"
#include "shader.h"
using namespace std;

const char *vertexPath = "vertex.glsl";
const char *fragmentPath = "fragment.glsl";

GLFWwindow* initGLFWAndGLAD(GLsizei width, GLsizei height, const GLchar* title, GLFWmonitor* monitor, GLFWwindow* share);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


void processInput(GLFWwindow *window);

// store window size
int display_w, display_h;
int SRC_WIDTH = 1200;
int SRC_HEIGHT = 1200;
bool whetherDrop = false;
bool whetherControlColor = false;

Curve curve;
int main()
{
	GLFWwindow* window = initGLFWAndGLAD(SRC_WIDTH, SRC_HEIGHT, "Bezier Curve", NULL, NULL);
	if (NULL == window) {
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetKeyCallback(window, key_callback);
	Shader shader(vertexPath, fragmentPath);
	cout << shader.ID << endl;
	curve.init(shader.ID, "pointPosition", "pointColor");
	GLfloat pointColor[] = { 1.0f, 0.0f, 1.0f };
	GLfloat curveColor[] = { 0.8f, 0.5f, 1.0f };
	
	
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::CreateContext();

	
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glfwPollEvents();
		
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::SetNextWindowSize(ImVec2(0,0));

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		shader.use();
		curve.setCurveColor(curveColor[0], curveColor[1], curveColor[2]);
		curve.setPointColor(pointColor[0], pointColor[1], pointColor[2]);
		curve.draw();

		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}





	//// optional: de-allocate all resources once they've outlived their purpose:
	//// ------------------------------------------------------------------------
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();

	return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// Èç¹ûÊ§°Ü return NULL
GLFWwindow* initGLFWAndGLAD(GLsizei width, GLsizei height, const GLchar* title, GLFWmonitor* monitor, GLFWwindow* share) {
	if (!glfwInit()) return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, title, monitor, share);
	if (NULL == window) {
		cout << "Failed to create GLFW Window" << endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwInit();
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}
	return window;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (whetherControlColor) {
		return;
	}
	if (GLFW_MOUSE_BUTTON_LEFT == button && GLFW_PRESS == action) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		curve.setPoint(xpos / display_w, ypos / display_h);
		whetherDrop = true;
	}
	else if (GLFW_MOUSE_BUTTON_RIGHT == button && GLFW_PRESS == action) {
		curve.deletePoint();
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	//    cout << "cursor_position_callback "<<xpos << " " << ypos << endl;
	if (whetherDrop && curve.whetherDropble()) {
		curve.setPoint(xpos / display_w, ypos / display_h);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (GLFW_KEY_P == key && GLFW_PRESS == action) {
		curve.SwitchMode(kProcess);
	}
}

