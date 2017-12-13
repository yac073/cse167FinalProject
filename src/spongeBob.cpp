#include "spongeBob.h"
#include <iostream>
#include "glm/ext.hpp"
#include "Window.h"

using namespace std;
using namespace glm;

OBJObjectz * SpongeBob::cube;

void SpongeBob::initializeOBJs() {
	cube = new OBJObjectz("../res/cube.obj");
}

SpongeBob::SpongeBob()
{
	mat4 S, T;
	initializeOBJs();
	body = new Geometryz("../res/cube.obj", cube, 1);
	left_leg = new Geometryz("../res/cube.obj", cube, 0);
	right_leg = new Geometryz("../res/cube.obj", cube, 0);

	S = scale(vec3(1.673, 2.482, 1.0));
	T = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.1f));
	body_T = new Transformz(T * S, body);

	S = scale(vec3(0.5, 1.05, 0.5));
	T = translate(mat4(1.0f), vec3(-0.7f, -3.5f, 0.0f));
	left_leg_T = new Transformz(T * S, left_leg);

	S = scale(vec3(0.5, 1.05, 0.5));
	T = translate(mat4(1.0f), vec3(0.7f, -3.5f, 0.0f));
	right_leg_T = new Transformz(T * S, right_leg);

	spongeBob_T = new Transformz(mat4(1.0f), body_T);
	spongeBob_T->addChild(left_leg_T);
	spongeBob_T->addChild(right_leg_T);

	spongeBob = new Groupz();
	spongeBob->addChild(spongeBob_T);

	toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -50.0f));
	x = y = z = 0.0f;
}

SpongeBob::~SpongeBob()
{

}

void SpongeBob::move(vec3 displacement)
{
	x += displacement.x;
	z += displacement.z;
}

void SpongeBob::draw(GLint shaderProg)
{
	spongeBob->draw(toWorld, 1, shaderProg);
}

void SpongeBob::update()
{
	angle += 0.1f;
	if (angle > 360) { angle = 0.0f; }
	//toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 10.0f)) * glm::rotate(glm::mat4(1.0f), glm::pi<float>() * angle / 180, glm::vec3(0.0f, 1.0f, 0.0f));

	spongeBob->update();
}

glm::vec3 SpongeBob::getPos()
{
	return glm::vec3(toWorld[3][0], 0.0f, toWorld[3][2]);
}

void SpongeBob::setHeight(float height)
{
	toWorld[3][1] = height + 0.5;
}
