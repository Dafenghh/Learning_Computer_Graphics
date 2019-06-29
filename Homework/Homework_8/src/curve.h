#ifndef CURVE_H_DAFENG
#define CURVE_H_DAFENG
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <numeric>
#include <cmath>
using std::vector;
using std::string;

typedef enum { kProcess, kAddPoint, kDragPoint } Mode;
class Curve {
private:
	static const float T_GAP;
	static const unsigned int T_NUMBER;
	static const float SELECT_THRESHOLD;

	string vec2Name;
	string colorVec3Name;
	unsigned int shaderID;
	unsigned int VAO;
	unsigned int VBO;

	vector<glm::vec2> points;
	vector<glm::vec2> curve;
	glm::vec3 pointColor;
	glm::vec3 curveColor;
	unsigned int findNearestPoint(glm::vec2 p);
	GLfloat distanceBetweenTwoPoint(glm::vec2 p1, glm::vec2 p2);
	void calculateCurve();
	void drawControlPoints();
	void drawPoints(const vector<glm::vec2> &points, GLfloat psize, glm::vec3 color, bool line);
	void drawCurve();
	void DrawProcess(float t);
	void setColorVec3(const glm::vec3 & p);
	glm::vec2 coorTransform(const double & x, const double & y);

	Mode mode_, last_mode_;
	float process_start_sec_;
	float process_total_sec_;

public:
	Curve();
	~Curve();
	void init(
		unsigned int,
		string,
		string
	);
	void setPointColor(GLfloat r, GLfloat g, GLfloat b);
	void setCurveColor(GLfloat r, GLfloat g, GLfloat b);
	void setPoint(const double & x, const double & y);
	void deletePoint();
	// 当已经选定四个点后，可以拖动四个点进行选择
	bool whetherDropble();
	void draw();
	void SwitchMode(Mode mode);
};

#endif 
