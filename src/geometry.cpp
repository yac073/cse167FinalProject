#include "geometry.h"

void Geometry::setMatrix(glm::mat4 m)
{
	M = m * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 15.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::pi<float>() * angle / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -15.0f));
	obj->setMatrix(M);
}

void Geometry::loadOBJ(char* s)
{
	obj = new OBJObject(s);
}

void Geometry::draw(glm::mat4 m)
{
	setMatrix(m);
	if (shouldDraw) { obj->draw(shaderProgram, isAlpha, name == "inter" ? true : false, id); }
	
}

void Geometry::tick()
{
	isAlpha = !isAlpha;
}

void Geometry::update()
{
}

Geometry::~Geometry()
{
	delete(obj);
}

