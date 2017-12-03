#include "Curve.h"
#include "Window.h"
using namespace std;
Curve::Curve() {
	setPointsToDefault();
	refreshPoints();
}

void Curve::refreshPoints()
{
	for (int i = 0; i < pointArraySize / 3; i++) {
		points[i * 3] = (points[(i * 3 - 1 + pointArraySize) % pointArraySize] + points[i * 3 + 1]) / 2.0f;
	}
	topPoint = glm::vec3(numeric_limits<float>::min());
	for (int i = 0; i < pointArraySize / 3; i++) {
		glm::vec3 p[4];
		for (int j = 0; j < 4; j++) {
			p[j] = points[(i * 3 + j) % pointArraySize];
		}
		glm::mat4x3 matrix(p[0].x, p[0].y, p[0].z,
			p[1].x, p[1].y, p[1].z,
			p[2].x, p[2].y, p[2].z,
			p[3].x, p[3].y, p[3].z);
		for (float t = 0; t < 1.0f; t += 0.005) {
			float c0 = pow(t, 0)* pow(1 - t, 3);
			float c1 = 3 * t*pow(1 - t, 2);
			float c2 = 3 * pow(t, 2)*pow(1 - t, 1);
			float c3 = pow(t, 3)*pow(1 - t, 0);
			glm::vec4 vector = glm::vec4(c0, c1, c2, c3);
			glm::vec3 q = matrix*vector;
			vertices.push_back(q);
			if (q.y > topPoint.y) {
				topPoint = glm::vec3(q);
				topIndex = vertices.size() - 1;
			}
		}
	}
	for (int i = 0; i < pointArraySize / 3; i++) {
		vertices.push_back(points[i * 3 + 2]);
		vertices.push_back(points[((i + 1) * 3 + 1) % pointArraySize]);
	}
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		sizeof(glm::vec3), // 3 * sizeof(GLfloat) Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Curve::setPointsToDefault()
{
	points[1] = glm::vec3(2.0f, 2.0f, -2.0f);
	points[2] = glm::vec3(4.0f, 4.0f, -4.0f);

	points[4] = glm::vec3(8.0f, 8.0f, -8.0f);
	points[5] = glm::vec3(10.0f, 6.0f, -10.0f);

	points[7] = glm::vec3(10.0f, 2.0f, -14.0f);
	points[8] = glm::vec3(8.0f, 0.0f, -16.0f);

	points[10] = glm::vec3(4.0f, -2.0f, -20.0f);
	points[11] = glm::vec3(2.0f, -4.0f, -22.0f);

	points[13] = glm::vec3(-2.0f, -8.0f, -22.0f);
	points[14] = glm::vec3(-4.0f, -6.0f, -20.0f);

	points[16] = glm::vec3(-8.0f, -2.0f, -16.0f);
	points[17] = glm::vec3(-10.0f, 0.0f, -14.0f);

	points[19] = glm::vec3(-10.0f, 4.0f, -10.0f);
	points[20] = glm::vec3(-8.0f, 6.0f, -8.0f);

	points[22] = glm::vec3(-4.0f, 6.0f, -4.0f);
	points[23] = glm::vec3(-2.0f, 4.0f, -2.0f);
}

Curve::~Curve()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Curve::draw(GLuint shaderProgram, bool hasFriction)
{
	glUseProgram(shaderProgram);
	glm::mat4 modelview = Window::V *toWorld;
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	uModel = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
	glBindVertexArray(VAO);
	glUniform1i(glGetUniformLocation(shaderProgram, "isCurve"), 1);
	if (!hasFriction) {
		glDrawArrays(GL_LINE_STRIP, 0, vertices.size() - 16);
	}
	else {
		glUniform1i(glGetUniformLocation(shaderProgram, "hasFric"), 1);
		glDrawArrays(GL_LINE_STRIP, 0, 199);
		glUniform1i(glGetUniformLocation(shaderProgram, "hasFric"), 0);
		glDrawArrays(GL_LINE_STRIP, 200, vertices.size() - 216);

	}
	
	glUniform1i(glGetUniformLocation(shaderProgram, "isCurve"), 0);
	for (int i = 0; i < pointArraySize / 3; i++) {
		glDrawArrays(GL_LINE_STRIP, vertices.size() - 16 + 2 * i, 2);
	}	
	glLineWidth(2.0f);
	glBindVertexArray(0);
}

void Curve::onPointsMoved()
{
	refreshPoints();
}

void Curve::refreshCurve()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	vertices.clear();
	refreshPoints();
}

