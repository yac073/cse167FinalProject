#ifndef _SPONGEBOB_H
#define _SPONGEBOB_H

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
#include "OBJObjectz.h"
#include "sceneGraph.h"

class SpongeBob
{
public:
	static OBJObjectz * cube;

	float angle = 0.0f;
	Groupz * spongeBob;

	Geometryz * body, *left_leg, *right_leg;

	Transformz * body_T, *left_leg_T, *right_leg_T;

	Transformz * spongeBob_T;

	static void initializeOBJs();

	SpongeBob();
	~SpongeBob();

	float x, y, z;
	glm::mat4 toWorld;
	void move(glm::vec3);

	void draw(GLint shaderProg);
	void update();
	glm::vec3 getPos();
	void setHeight(float);
	bool shouldMove = false;
};
#endif
