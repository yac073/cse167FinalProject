#ifndef _CURVE_H_
#define _CURVE_H_
#define GLFW_INCLUDE_GLEXT
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
class Curve {
public:
	const static int pointArraySize = 24;
	glm::mat4 toWorld = glm::mat4(1.0f);
	glm::vec3 points[pointArraySize];
	std::vector<glm::vec3> vertices;
	GLuint VBO, VAO;
	GLuint uProjection, uModelview, uModel;
	glm::vec3 topPoint;
	int topIndex;
	int index;
	Curve();
	void refreshPoints();
	void setPointsToDefault();
	~Curve();
	void draw(GLuint shaderProgram, bool);
	void onPointsMoved();
	void refreshCurve();
};
#endif