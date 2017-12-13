#include "sceneGraph.h"
#include <iostream>
#include "glm/ext.hpp"
#include "Window.h"

using namespace std;
using namespace glm;


Groupz::Groupz()
{

}

Groupz::~Groupz()
{

}

void Groupz::addChild(Nodez * child)
{
	children.push_back(child);
}

void Groupz::draw(glm::mat4 C, bool show, GLint shaderProg)
{
	children.front()->draw(C, show, shaderProg);
}

void Groupz::update()
{
	children.front()->update();
}

void Groupz::update(mat4 R)
{

}

Transformz::Transformz(glm::mat4 M, Nodez * child)
{
	Mo = M;
	children.push_back(child);

	count = 0;

	// ec
	angle = initial_angle = -45.0f;
	range = 90.0f;
	velocity = 1.0f;
}

void Transformz::updateMo(glm::mat4 M)
{
	Mo = M * Mo;
}

Transformz::~Transformz()
{

}

void Transformz::addChild(Nodez * child)
{
	children.push_back(child);
}

void Transformz::removeChild(Nodez * child)
{
	children.remove(child);
}

void Transformz::draw(glm::mat4 C, bool show, GLint shaderProg)
{
	for (list<Nodez *>::iterator children_it = children.begin(); children_it != children.end(); children_it++)
	{
		(*children_it)->draw(C * Mo, show, shaderProg);
	}
}

void Transformz::update()
{
	for (list<Nodez *>::iterator children_it = children.begin(); children_it != children.end(); children_it++)
	{
		(*children_it)->update();
	}
}

void Transformz::update(mat4 R)
{
	for (list<Nodez *>::iterator children_it = children.begin(); children_it != children.end(); children_it++)
	{
		(*children_it)->update(R);
	}
}

void Transformz::update(bool clockwise)
{
	count++;
	mat4 O;
	float time_span = range / velocity * 2;
	if ((count % (int)time_span) < (time_span / 2)) {
		O = rotate(mat4(1.0f), (angle += velocity) / 180.0f * pi<float>(), vec3(-1.0f, 0.0f, 0.0f));
	}
	else if ((count % (int)time_span) < time_span) {
		O = rotate(mat4(1.0f), (angle -= velocity) / 180.0f * pi<float>(), vec3(-1.0f, 0.0f, 0.0f));
	}

	O = (clockwise ? O : inverse(O));
	for (list<Nodez *>::iterator children_it = children.begin(); children_it != children.end(); children_it++)
	{
		(*children_it)->update(O);
	}
}

Geometryz::Geometryz(const char *filepath, OBJObjectz * object, bool body_or_leg)
{
	bl = body_or_leg;
	obj = object;

	initalizeAllOtherFields();

	obj->toCenterAndStandardSize(s, x, y, z);
}

void Geometryz::initalizeAllOtherFields()
{
	s = 1.0f;
	x = y = z = o = 0.0f;

	toWorld = Mo = mat4(1.0f);
}

Geometryz::~Geometryz()
{

}

void Geometryz::setModelMatrix(glm::mat4 M)
{
	Mo = M;
}

void Geometryz::draw(glm::mat4 M, bool show, GLint shaderProg)
{
	setModelMatrix(M);

	if (show)
		obj->draw(toWorld, shaderProg, bl);
}

void Geometryz::update()
{
	toWorld = mat4(1.0f);
	mat4 S = scale(vec3(s, s, s));
	mat4 T = translate(mat4(1.0f), vec3(x, y, z));
	toWorld = Mo * T * S * toWorld;
}

void Geometryz::update(mat4 R)
{
	toWorld = mat4(1.0f);
	mat4 S = scale(vec3(s, s, s));
	mat4 T = translate(mat4(1.0f), vec3(x, y, z));
	toWorld = Mo * R * T * S * toWorld;
}
