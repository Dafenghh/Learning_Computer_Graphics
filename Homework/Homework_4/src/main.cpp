#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

#include <iostream>
using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* glsl_version = "#version 330";
int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
														   //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

														   // Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);



	// build and compile our shader zprogram
	// ------------------------------------
	Shader shader("vertex.glsl", "fragment.glsl");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-2.0f, -2.0f, -2.0f,  0.0f, 0.0f,
		 2.0f, -2.0f, -2.0f,  1.0f, 0.0f,
		 2.0f,  2.0f, -2.0f,  1.0f, 1.0f,
		 2.0f,  2.0f, -2.0f,  1.0f, 1.0f,
		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 0.0f,

		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f,
		 2.0f, -2.0f,  2.0f,  1.0f, 0.0f,
		 2.0f,  2.0f,  2.0f,  1.0f, 1.0f,
		 2.0f,  2.0f,  2.0f,  1.0f, 1.0f,
		-2.0f,  2.0f,  2.0f,  0.0f, 1.0f,
		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f,

		-2.0f,  2.0f,  2.0f,  1.0f, 0.0f,
		-2.0f,  2.0f, -2.0f,  1.0f, 1.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f,
		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f,
		-2.0f,  2.0f,  2.0f,  1.0f, 0.0f,

		2.0f,  2.0f,  2.0f,  1.0f, 0.0f,
		2.0f,  2.0f, -2.0f,  1.0f, 1.0f,
		2.0f, -2.0f, -2.0f,  0.0f, 1.0f,
		2.0f, -2.0f, -2.0f,  0.0f, 1.0f,
		2.0f, -2.0f,  2.0f,  0.0f, 0.0f,
		2.0f,  2.0f,  2.0f,  1.0f, 0.0f,

		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f,
		2.0f, -2.0f, -2.0f,  1.0f, 1.0f,
		2.0f, -2.0f,  2.0f,  1.0f, 0.0f,
		2.0f, -2.0f,  2.0f,  1.0f, 0.0f,
		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f,

		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f,
		2.0f,  2.0f, -2.0f,  1.0f, 1.0f,
		2.0f,  2.0f,  2.0f,  1.0f, 0.0f,
		2.0f,  2.0f,  2.0f,  1.0f, 0.0f,
		-2.0f,  2.0f,  2.0f,  0.0f, 0.0f,
		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// load and create a texture 
	// -------------------------
	unsigned int texture1, texture2;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
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
	// texture 2
	// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	shader.setMat4("projection", projection);

	bool can_rotate = false;
	bool move_horizontal = false;
	bool move_vertical = false;
	bool can_scale = false;

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::SetNextWindowSize(ImVec2(0, 0));

		{
			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
			ImGui::Checkbox("Rotate", &can_rotate);
			ImGui::Checkbox("Move Vertical", &move_vertical);
			ImGui::Checkbox("Move Horizontal", &move_horizontal);
			ImGui::Checkbox("Scale", &can_scale);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
															// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// activate shader
		shader.use();

		int translate_range = 10;
		float cur_time = (float)glfwGetTime();
		int segment = static_cast<int>(cur_time / translate_range);
		float pos = cur_time - segment * translate_range - translate_range / 2.0f;
		if (segment % 2 == 1) {
			pos = -pos;
		}
		pos *= 2;
		
		// create transformations
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 view = glm::mat4(1.0f);
		
		float scale_size = cos(cur_time) + 1.5f;
		if (can_scale) {
			model = glm::scale(model, glm::vec3(scale_size, scale_size, scale_size));
		}

		if (move_vertical) {
			model = glm::translate(model, glm::vec3(0.0f, pos, 0.0f));
		}
		if (move_horizontal) {
			model = glm::translate(model, glm::vec3(pos, 0.0f, 0.0f));
		}
		if (can_rotate) {
			model = glm::rotate(model, cur_time * glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		}
		if (can_scale) {
			model = glm::scale(model, glm::vec3(scale_size, scale_size, scale_size));
		}
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -50.0f));
		
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		// render box
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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