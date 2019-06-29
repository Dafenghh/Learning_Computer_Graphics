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
#include "camera.h"
#include <iostream>
using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* glsl_version = "#version 330";


class MyClock {
	const bool& changing;
	bool last_changing;
	float last_time = 0.0f;
	float offset = 0.0f;
public:
	float GetTime() {
		if (!changing) {
			last_changing = false;
			return last_time;
		}

		if (last_changing) {
			return last_time = glfwGetTime() - offset;
		}

		last_changing = true;
		offset = glfwGetTime() - last_time;
		return last_time;
	}
	MyClock(const bool &changing) :changing(changing) { }
};

bool user_control = false;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 20.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

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

#define CUBE_COLOR1 0.8f, 0.1f, 0.2f
#define CUBE_COLOR2 0.0f, 0.7f, 0.1f
#define CUBE_COLOR3 0.0f, 0.8f, 0.6f
#define CUBE_COLOR4 0.4f, 0.3f, 0.9f
#define CUBE_COLOR5 0.9f, 0.8f, 0.9f
#define CUBE_COLOR6 0.9f, 0.1f, 0.9f
	float vertices[] = {
		-2.0f, -2.0f, -2.0f,  0.0f, 0.0f, CUBE_COLOR1,
		 2.0f, -2.0f, -2.0f,  1.0f, 0.0f, CUBE_COLOR1,
		 2.0f,  2.0f, -2.0f,  1.0f, 1.0f, CUBE_COLOR1,
		 2.0f,  2.0f, -2.0f,  1.0f, 1.0f, CUBE_COLOR1,
		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f, CUBE_COLOR1,
		-2.0f, -2.0f, -2.0f,  0.0f, 0.0f, CUBE_COLOR1,

		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f, CUBE_COLOR2,
		 2.0f, -2.0f,  2.0f,  1.0f, 0.0f, CUBE_COLOR2,
		 2.0f,  2.0f,  2.0f,  1.0f, 1.0f, CUBE_COLOR2,
		 2.0f,  2.0f,  2.0f,  1.0f, 1.0f, CUBE_COLOR2,
		-2.0f,  2.0f,  2.0f,  0.0f, 1.0f, CUBE_COLOR2,
		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f, CUBE_COLOR2,

		-2.0f,  2.0f,  2.0f,  1.0f, 0.0f, CUBE_COLOR3,
		-2.0f,  2.0f, -2.0f,  1.0f, 1.0f, CUBE_COLOR3,
		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f, CUBE_COLOR3,
		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f, CUBE_COLOR3,
		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f, CUBE_COLOR3,
		-2.0f,  2.0f,  2.0f,  1.0f, 0.0f, CUBE_COLOR3,

		2.0f,  2.0f,  2.0f,  1.0f, 0.0f, CUBE_COLOR4,
		2.0f,  2.0f, -2.0f,  1.0f, 1.0f, CUBE_COLOR4,
		2.0f, -2.0f, -2.0f,  0.0f, 1.0f, CUBE_COLOR4,
		2.0f, -2.0f, -2.0f,  0.0f, 1.0f, CUBE_COLOR4,
		2.0f, -2.0f,  2.0f,  0.0f, 0.0f, CUBE_COLOR4,
		2.0f,  2.0f,  2.0f,  1.0f, 0.0f, CUBE_COLOR4,

		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f, CUBE_COLOR5,
		2.0f, -2.0f, -2.0f,  1.0f, 1.0f,  CUBE_COLOR5,
		2.0f, -2.0f,  2.0f,  1.0f, 0.0f,  CUBE_COLOR5,
		2.0f, -2.0f,  2.0f,  1.0f, 0.0f,  CUBE_COLOR5,
		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f, CUBE_COLOR5,
		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f, CUBE_COLOR5,

		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f, CUBE_COLOR6,
		2.0f,  2.0f, -2.0f,  1.0f, 1.0f,  CUBE_COLOR6,
		2.0f,  2.0f,  2.0f,  1.0f, 0.0f,  CUBE_COLOR6,
		2.0f,  2.0f,  2.0f,  1.0f, 0.0f,  CUBE_COLOR6,
		-2.0f,  2.0f,  2.0f,  0.0f, 0.0f, CUBE_COLOR6,
		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f, CUBE_COLOR6
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

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


	bool rotate_cube = false;
	bool move_horizontal = false;
	bool move_vertical = false;
	bool can_scale = false;
	bool ortho = false;
	bool rotate_camera = false;
	
	MyClock clock_cube(rotate_cube);
	MyClock clock_camera(rotate_camera);


	float ortho_param[6] = {-6.0f, 6.0f, -6.0f, 6.0f, 0.1f, 100.0f};
	float perspect_param[3] = {45.0f, 0.1f, 100.0f};

	
	float cube_position[3] = {};

	float radius = 20.0f;
	float camX = radius, camZ = 0;


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::SetNextWindowSize(ImVec2(600, 300));

		{
			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
			ImGui::Checkbox("Rotate Cube", &rotate_cube);
			ImGui::Checkbox("Move Vertical", &move_vertical);
			ImGui::Checkbox("Move Horizontal", &move_horizontal);
			ImGui::Checkbox("Scale", &can_scale);
			ImGui::Checkbox("Rotate Camera", &rotate_camera);
			ImGui::Checkbox("Orthographic Projection", &ortho);
			ImGui::InputFloat2("Left and Right For Ortho", ortho_param);
			ImGui::InputFloat2("Bottom and Up  For Ortho", ortho_param + 2);
			ImGui::InputFloat2("Near and Far   For Ortho", ortho_param + 4);
			ImGui::InputFloat3("Param For Perspective", perspect_param);
			ImGui::InputFloat3("Cube Position", cube_position);
			ImGui::Checkbox("User Control", &user_control);
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

		int translate_range = 20;
		float cur_time = (float)glfwGetTime();
		int segment = static_cast<int>(cur_time / translate_range);
		float pos = cur_time - segment * translate_range - translate_range / 2.0f;
		if (segment % 2 == 1) {
			pos = -pos;
		}
		
		// create transformations
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
				
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

		model = glm::translate(model, glm::vec3(cube_position[0], cube_position[1], cube_position[2]));

		model = glm::rotate(model, clock_cube.GetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		shader.setMat4("model", model);

		if (user_control) {
			glm::mat4 view = camera.GetViewMatrix();
			shader.setMat4("view", view);
			
			glm::mat4 projection = glm::perspective(glm::radians(camera.zoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			shader.setMat4("projection", projection);
		}
		else {
			float camera_radian = clock_camera.GetTime();
			camX = sin(camera_radian) * radius;
			camZ = cos(camera_radian) * radius;
			glm::mat4 view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
			shader.setMat4("view", view);

			glm::mat4 projection(1.0f);
			if (ortho) {
				projection = glm::ortho(ortho_param[0], ortho_param[1], ortho_param[2], ortho_param[3], ortho_param[4], ortho_param[5]);
			}
			else {
				projection = glm::perspective(perspect_param[0], (float)SCR_WIDTH / (float)SCR_HEIGHT, perspect_param[1], perspect_param[2]);
			}
			shader.setMat4("projection", projection);
		}




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
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		user_control ^= 1;

	if (!user_control) return;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (!user_control) return;
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (!user_control) return;
	camera.ProcessMouseScroll(yoffset);
}