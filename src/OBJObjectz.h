#ifndef OBJOBJECTZ_H
#define OBJOBJECTZ_H

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Light.h"
#include <string.h>
#include "math.h"


class OBJObjectz
{
private:
	// object properties
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<std::vector<std::pair<int, int>>> faces;

	// modern OpenGL variables
	GLfloat * GLvertices;
	GLfloat * GLnormals;
	GLuint * GLfaces;
	GLuint VBO_vertices, VBO_normals, VAO, EBO;
	GLuint uProjection, uModel, uView;

	

public:
	static GLuint bodyTexture, legTexture;
	OBJObjectz(const char* filepath);
	~OBJObjectz();

	void parse(const char* filepath);
	void loadArrays();
	void setModernOpenGL();

	void draw(glm::mat4 toWorld, GLint shaderProgram, bool sphere);

	void toCenterAndStandardSize(float &s, float &x, float &y, float &z);

	void toCenterAndStandardSize(float &s, float &x, float &y, float &z, glm::vec3 &, float &);

	static void loadSpongeBobTextures();
};

#endif