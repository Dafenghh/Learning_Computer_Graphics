#include "cg_tools.h"
#include <vector>
#include <algorithm>
using namespace std;


void drawTriangle(int[], int[]);
void drawCircle(const int, const int, const int);
vector<GLfloat> generateLine(int, int, int, int, int = 1);
void pushPointToVector(vector<GLfloat> & points, int x, int y);
void push8Points(vector<GLfloat> & points, int x, int y, int x_offset = 0, int y_offset = 0);
void drawVectorPoints(vector<GLfloat> & points);

int display_w=400, display_h=400;


int main()
{	
	CgTools cg;
	cg.Init();

	int shaderProgram = cg.BuildShaderProgram("vertex_shader.txt", "fragment_shader.txt");
	GLFWwindow * window = cg.window;

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	int operation = 1; // simple triangle
					   // make points bigger
	glPointSize(2.0f);
	// make line wider
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(100.0f);
	int x[] = { 100, 200, 300 };
	int y[] = { 300, 200, 300 };
	int c[] = { 100, 100, 100 };
	int p0[] = { 100, 300 };
	int p1[] = { 200, 200 };
	int p2[] = { 300, 300 };

	ImVec4 color = ImVec4(0.45f, 0.15f, 0.60f, 1.00f);

	cg.Start([&]() {


		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			ImGui::Begin("Homework 2 Menu", NULL, ImGuiWindowFlags_MenuBar);
			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("Choose Shape")) {
					if (ImGui::MenuItem("Draw Triangle", "")) {
						operation = 1;
					}
					if (ImGui::MenuItem("Draw Circle", "")) {
						operation = 2;
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			ImGui::InputInt3("(X, Y, R) of Circle", c);
			ImGui::InputInt2("(X, Y) of Point 0 For Triangle", p0);
			ImGui::InputInt2("(X, Y) of Point 1 For Triangle", p1);
			ImGui::InputInt2("(X, Y) of Point 2 For Triangle", p2);
			ImGui::End();
		}
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindVertexArray(VAO);

		// Rendering
		ImGui::Render();
		
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glUseProgram(shaderProgram);

		if (1 == operation) {
			x[0] = p0[0];
			y[0] = p0[1];
			x[1] = p1[0];
			y[1] = p1[1];
			x[2] = p2[0];
			y[2] = p2[1];
			drawTriangle(x, y);
		}
		else if (2 == operation) {
			drawCircle(c[0], c[1], c[2]);
		}

	});

	



	//// optional: de-allocate all resources once they've outlived their purpose:
	//// ------------------------------------------------------------------------
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);




	return 0;
}
void pushPointToVector(vector<GLfloat> & points, int x, int y) {
	points.push_back((GLfloat)x / display_w);
	points.push_back((GLfloat)y / display_h);
	points.push_back(1.0f);
}
void push8Points(vector<GLfloat> & points, int x, int y, int x_offset, int y_offset) {
	pushPointToVector(points, x_offset + x, y_offset + y);
	pushPointToVector(points, x_offset + x, y_offset - y);
	pushPointToVector(points, x_offset - x, y_offset + y);
	pushPointToVector(points, x_offset - x, y_offset - y);
	pushPointToVector(points, x_offset + y, y_offset + x);
	pushPointToVector(points, x_offset + y, y_offset - x);
	pushPointToVector(points, x_offset - y, y_offset + x);
	pushPointToVector(points, x_offset - y, y_offset - x);
}


vector<GLfloat> generateLine(int x0, int y0, int x1, int y1, int dxstep) {
	vector<GLfloat> result = vector<GLfloat>();
	// draw (x_0, y_0);
	pushPointToVector(result, x0, y0);
	// caculate k
	if (std::abs(x1 - x0) < dxstep) {
		// k does not exist
		int max = std::max(y0, y1);
		int min = std::min(y0, y1);
		for (GLfloat i = min + dxstep; i < max; i += dxstep) {
			pushPointToVector(result, x0, i);
		}
	}
	else {
		// k exist
		// make x0 smaller than x1
		bool whetherBiggerThan1 = abs(y1 - y0) > abs(x1 - x0);
		if (whetherBiggerThan1) {
			std::swap(x0, y0);
			std::swap(x1, y1);
		}
		if (x0 > x1) {
			std::swap(x0, x1);
			std::swap(y0, y1);
		}
		int delta_x = x1 - x0;
		int delta_y = std::abs(y1 - y0);
		int delta_x_2 = 2 * delta_x;
		int delta_y_2 = 2 * delta_y;
		int p = delta_x_2;
		int dystep;
		if (y0 < y1) {
			dystep = dxstep;
		}
		else {
			dystep = -1 * dxstep;
		}
		for (int x = x0, y = y0; x < x1; x += dxstep) {
			if (whetherBiggerThan1) {
				pushPointToVector(result, y, x);
			}
			else {
				pushPointToVector(result, x, y);
			}
			p = p - delta_y_2;
			if (p < 0) {
				y += dystep;
				p += delta_x_2;
			}
		}
	}
	return result;
}

void drawVectorPoints(vector<GLfloat> & points) {
	GLuint point_count = (GLuint)points.size() / 3;
	GLuint *indices = new GLuint[point_count];
	for (int i = 0; i < point_count; ++i) {
		indices[i] = i;
	}
	GLfloat *vertices = new GLfloat[point_count * 3];
	std::copy(points.begin(), points.end(), vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * point_count, vertices, GL_STREAM_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*point_count, indices, GL_STREAM_DRAW);
	glDrawElements(GL_POINTS, point_count, GL_UNSIGNED_INT, 0);
	delete[]indices;
	delete[]vertices;
}

void drawCircle(const int c_x, const int c_y, const int r) {
	vector<GLfloat> points;
	//    int p = 2 * r - 3;
	int p = 5 - 4 * r;
	push8Points(points, 0, r, c_x, c_y);
	for (int x = 1, y = r; x <= y; ++x) {
		push8Points(points, x, y, c_x, c_y);
		if (p >= 0) {
			//            p -= 4 * x + 6;
			p += 8 * x - 8 * y + 20;
			--y;
		}
		else {
			//            p -= 4 * (x - y) + 10;
			p += 8 * x + 12;
			//            --y;
		}
	}
	drawVectorPoints(points);
}

void drawTriangle(int x[], int y[]) {
	vector<GLfloat> allPoints = vector<GLfloat>();
	for (int i = 0; i < 3; ++i) {
		for (int j = i + 1; j < 3; ++j) {
			vector<GLfloat> temp = generateLine(x[i], y[i], x[j], y[j]);
			allPoints.insert(allPoints.end(), temp.begin(), temp.end());
		}
	}
	drawVectorPoints(allPoints);
	//    vector<GLfloat> temp = generateLine(x[0], y[0], x[2], y[2], 0.001);
	//    allPoints.insert(allPoints.end(), temp.begin(), temp.end());
}