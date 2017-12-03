#include "tansform.h"

void Transform::addChild(Node * n)
{
	children->push_back(n);
}

void Transform::removeChild(Node * n)
{
	children->remove(n);
}

void Transform::draw(glm::mat4 C)
{
	glm::mat4 NEW_M = C * M;
	for each ( Node* var in *children)
	{
		var->draw(NEW_M);
	}
}

Transform::~Transform()
{
	for each(Node* var in *children) {
		delete(var);
	}
	delete(children);
}

void Transform::update()
{
	for each(Node* var in *children) {
		var->update();
	}
}

void Transform::tick()
{
	for each(Node* node in *children) {
		node->tick();
	}
}

