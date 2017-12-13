#ifndef LIGHTZ_H
#define LIGHTZ_H

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
#include <string.h>
#include "math.h"

class DirectionLight
{
public:
	// modern OpenGL variables
	static GLuint uLight_color, uLight_direction;

	// light properties

	static glm::vec3 direction;
	static glm::vec3 color;
	float angle;

	DirectionLight();
	~DirectionLight();

	void update();

};

#endif