#include "spongeBob.h"
#include <iostream>
#include "glm/ext.hpp"
#include "Window.h"

using namespace std;
using namespace glm;

OBJObjectz * SpongeBob::cube;
int moveNum = 0;
vec3 dir(0.0f);
int i = 1;

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
	spongeBob->update();
	if (shouldMove) {
		if (moveNum == 0) {
			moveNum = rand() % 10 + 1;
			int pn1 = rand() % 2;
			int pn2 = rand() % 2;
			dir = normalize(vec3((rand() % 10 + 1) * (pn1 == 1? -1 :1), 0.0f, (rand() % 10+ 1) * (pn2 == 1 ? -1 : 1)));
		}
		else {
			moveNum--;
			toWorld = toWorld * glm::translate(glm::mat4(1.0f), dir);
			toWorld[3][0] = glm::clamp(toWorld[3][0], -390.0f, 390.0f);
			toWorld[3][2] = glm::clamp(toWorld[3][2], -390.0f, 390.0f);
			
			//cout << toWorld[3][0] << " " << toWorld[3][2] << " " << i << endl;			
		}
	}
}

glm::vec3 SpongeBob::getPos()
{
	return glm::vec3(toWorld[3][0], 0.0f, toWorld[3][2]);
}

void SpongeBob::setHeight(float height)
{
	toWorld[3][1] = height + 0.5;
}
