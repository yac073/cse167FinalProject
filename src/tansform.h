#pragma once
#include "node.h"
#include <stdlib.h>
#include <list>
using namespace std;
class Transform : public Node {
public:
	glm::mat4 M;
	list<Node *>* children;
	char * name;
	void addChild(Node*);
	void removeChild(Node *);
	void draw(glm::mat4 C);
	Transform(char* n="") : Node() {
		M = glm::mat4(1.0f);
		children = new list<Node*>;
		name = n;
	};
	~Transform();
	void update();
	void tick();
};