#include "curve.h"
#include <iostream>
using std::cout;
using std::endl;
const float Curve::T_GAP = 0.001;
const unsigned int Curve::T_NUMBER = 1 / T_GAP + 1;
const float Curve::SELECT_THRESHOLD = 0.1;
Curve::Curve() {
	pointColor = glm::vec3(1.0f);
	curveColor = glm::vec3(1.0f, 0.5f, 0.0f);
	SwitchMode(kAddPoint);
	process_total_sec_ = 15;
}

void Curve::init(
	unsigned int _shaderID,
	string _vec2Name,
	string _colorVec3Name) {
	shaderID = _shaderID;
	vec2Name = _vec2Name;
	colorVec3Name = _colorVec3Name;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	curve = vector<glm::vec2>(T_NUMBER);
}
Curve::~Curve() {
	/**
	* Ïú»Ùbuffer
	*/
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VBO);
}

void Curve::setPoint(const double & x, const double & y) {
	glm::vec2 temp = coorTransform(x, y);
	points.push_back(temp);
	calculateCurve();
}

unsigned int Curve::findNearestPoint(glm::vec2 p) {
	GLfloat minDistance = distanceBetweenTwoPoint(p, points[0]);
	unsigned int minPointIndex = 0;
	for (unsigned int i = 1; i < points.size(); ++i) {
		GLfloat distanceTemp = distanceBetweenTwoPoint(p, points[i]);
		if (distanceTemp < minDistance) {
			minDistance = distanceTemp;
			minPointIndex = i;
		}
	}
	return minPointIndex;
}

GLfloat Curve::distanceBetweenTwoPoint(glm::vec2 p1, glm::vec2 p2) {
	GLfloat d_x = p1.x - p2.x;
	GLfloat d_y = p1.y - p2.y;
	return d_x * d_x + d_y * d_y;
}

void Curve::draw() {
	glBindVertexArray(VAO);
	drawControlPoints();
	drawCurve();

	if (mode_ == kProcess) {
		float sec = (float)glfwGetTime() - process_start_sec_;
		if (sec > process_total_sec_) {
			SwitchMode(last_mode_);
			return;
		}
		DrawProcess(sec / process_total_sec_);
	}
}

void Curve::drawControlPoints() {
	if (points.empty()) return;
	drawPoints(points, 15.0f, pointColor, true);
/*	for (unsigned int i = 0; i < points.size(); ++i) {
		setPositionVec2(points[i]);
		glDrawArrays(GL_POINTS, 0, 1);
	}*/
}

void Curve::DrawProcess(float t) {
	vector<glm::vec2> points = this->points;
	while (points.size() > 1) {
		vector<glm::vec2> next_points;
		for (size_t i = 0; i + 1 < points.size(); i++) {
			next_points.push_back((1 - t)*points[i] + t * points[i + 1]);
		}
		glm::vec3 color(0.8f, 0.9f, 0.3f);
		if (next_points.size() == 1) color = curveColor;
		drawPoints(next_points, 10.0f, color, true);
		points = next_points;
	}
}

void Curve::drawPoints(const vector<glm::vec2> &points, GLfloat psize, glm::vec3 color, bool line = false) {
	glPointSize(psize);
	this->setColorVec3(color);
	GLfloat *vertices = new GLfloat[points.size() * 2];
	GLuint *indices = new GLuint[points.size()];
	for (unsigned int i = 0; i < points.size(); ++i) {
		vertices[i * 2] = points[i].x;
		vertices[i * 2 + 1] = points[i].y;
		indices[i] = i;
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * points.size(), vertices, GL_STREAM_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*points.size(), indices, GL_STREAM_DRAW);
	glDrawElements(GL_POINTS, points.size(), GL_UNSIGNED_INT, 0);

	delete[] indices;

	if (points.size() >= 2 && line) {
		glPointSize(1.0f);
		this->setColorVec3(glm::vec3(1.0f));
		indices = new GLuint[(points.size() - 1) * 2];
		for (int i = 0; i < points.size() - 1; i++) {
			indices[i * 2] = i;
			indices[i * 2 + 1] = i + 1;
		}
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*2*(points.size()-1), indices, GL_STREAM_DRAW);
		glDrawElements(GL_LINES, 2 * (points.size() - 1), GL_UNSIGNED_INT, 0);
		delete[] indices;
	}

	delete[] vertices;
}


void Curve::drawCurve() {
	if (points.size() <= 1) return;
	drawPoints(curve, 1.5f, curveColor);
}

void Curve::setColorVec3(const glm::vec3 & p) {
	glUniform3fv(glGetUniformLocation(shaderID, colorVec3Name.c_str()), 1, glm::value_ptr(p));
}

void Curve::calculateCurve() {
	if (points.size() <= 1) return;
	for (unsigned int i = 0; i < T_NUMBER; ++i) {
		GLfloat t = i * T_GAP;
		curve[i] = glm::vec2(0, 0);
		float co = 1;
		int n = points.size() - 1;
		for (int j = 0; j <= n; j++) {
			if (j > 0) {
				co *= n - j + 1;
				co /= j;
			}
			curve[i] += co * pow(t, j) * pow(1-t,n-j) * points[j];
		}
	}
}

bool Curve::whetherDropble() {
	return false;
}

void Curve::setPointColor(GLfloat r, GLfloat g, GLfloat b) {
	pointColor.x = r;
	pointColor.y = g;
	pointColor.z = b;
}

void Curve::setCurveColor(GLfloat r, GLfloat g, GLfloat b) {
	curveColor.x = r;
	curveColor.y = g;
	curveColor.z = b;
}

void Curve::deletePoint() {
	if (points.empty()) return;
	points.pop_back();
	calculateCurve();
}

glm::vec2 Curve::coorTransform(const double & x, const double & y) {
	return glm::vec2(2 * x - 1.0, -2 * y + 1);
}


void Curve::SwitchMode(Mode mode) {
	mode_ = mode;
	if (mode == kProcess) {
		process_start_sec_ = (float)glfwGetTime();
	}
	else {
		last_mode_ = mode_;
	}
}
