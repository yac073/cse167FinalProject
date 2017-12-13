#pragma once
#include "node.h"
#include <string>
#include "OBJObject.h"
using namespace std;
class Geometry : public Node{
public:
	OBJObject* obj;
	GLint shaderProgram;
	glm::mat4 M;
	char * name;
	void setMatrix(glm::mat4);
	void loadOBJ(char*);
	void draw(glm::mat4);
	bool direction;
	bool isAlpha;
	bool shouldDraw;
	int maxAngle;
	int presetAngle;
	int speed;
	int id;
	float angle;
	Geometry() : Node(){
		obj = nullptr;
	};
	Geometry(OBJObject* o, GLint shader, int id, char* n ="", bool initialDir = true) : Node() {
		maxAngle = 60;
		speed = 1;
		obj = o;
		shaderProgram = shader;
		name = n;
		shouldDraw = true;
		this->id = id;
		//obj->id = id;
	};
	void tick();
	void update();
	~Geometry();
};