#ifndef _SCENEGRAPH_H_
#define _SCENEGRAPH_H_

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
#include <iostream>
#include "glm/ext.hpp"
#include <vector>
#include <list>


class Nodez
{
public:
	virtual void draw(glm::mat4 M, bool show, GLint shaderProg) = 0;

	virtual void update() = 0;

	virtual void update(glm::mat4 R) = 0;
};

class Groupz : public Nodez
{
public:
	std::list <Nodez *> children;

	Groupz();
	~Groupz();
	void addChild(Nodez * child);
	void draw(glm::mat4 C, bool show, GLint shaderProg);
	void update();
	void update(glm::mat4 R);
};

class Transformz : public Nodez
{
public:
	glm::mat4 Mo;
	std::list <Nodez *> children;

	int count;
	float angle;
	// ec
	float initial_angle;
	float range;
	float velocity;

	Transformz(glm::mat4 M, Nodez * child);
	~Transformz();

	void addChild(Nodez * child);
	void removeChild(Nodez * child);
	void draw(glm::mat4 C, bool show, GLint shaderProg);
	void update();
	void update(glm::mat4 R);
	void update(bool clockwise);
	void updateMo(glm::mat4 M);
};


class Geometryz : public Nodez
{
public:
	OBJObjectz * obj;
	bool bl;
	glm::mat4 toWorld;
	glm::mat4 Mo;

	GLint shaderProgram;

	// object fields
	float x, y, z, s, o;

	Geometryz(const char* filepath, OBJObjectz *, bool);
	~Geometryz();

	void initalizeAllOtherFields();

	void setModelMatrix(glm::mat4 M);

	void draw(glm::mat4 M, bool show, GLint shaderProg);
	void update();
	void update(glm::mat4 R);
};

#endif