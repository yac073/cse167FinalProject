#ifndef _LSYSTEM_H_
#define _LSYSTEM_H_
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <climits>
#include "tansform.h"
#include "geometry.h"
#include "OBJObject.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
using namespace std;
class lSystem {
	string w;	
	vector<pair<char, string>>* V;
	vector<pair<char, int>>* S;
	int index2;
	const int randNum = 3;
private:
	void buildChild(OBJObject*, glm::vec3, GLint, glm::vec3, Transform*);
public:
	lSystem();
	~lSystem();
	void addV(char, string);
	void addS(char, int);
	string getString();
	void setString(string);
	void iterate(int num = 1);
	Transform* getResult(OBJObject*, GLint);
};
#endif 
