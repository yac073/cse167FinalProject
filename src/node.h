#pragma once
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
class Node {
public: 
	virtual void draw(glm::mat4) = 0;
	virtual void update() = 0;
	virtual void tick() = 0;
};