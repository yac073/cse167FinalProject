#ifndef _TREE_H
#define _TREE_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lSystem.h"
#include "tansform.h"
class LTree {
private:
	lSystem* tree;
	Transform* nodeTree;
	OBJObject* obj;
	GLint shader;
	glm::vec3 pos;
public:
	LTree(int, OBJObject*, GLint,int);
	~LTree();
	void draw();
	void updateHeight(float);
	glm::vec3 getPos();
	void updateTree();
};
#endif
