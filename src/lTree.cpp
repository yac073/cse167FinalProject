#include "lTree.h"

LTree::LTree(int iTRtime, OBJObject* obj, GLint shader, int type)
{
	tree = new lSystem();
	tree->setString("X");
	tree->addS('+', 1);
	tree->addS('-', 0);
	tree->addS('*', 2);
	tree->addS('/', 3);
	if (type == 0) {
		tree->addV('X', "F-[[X]+X]++[[X]+X]-*[[X]+X]//[[X]+X]");
	}
	else if (type == 1) {
		tree->addV('X', "F[--X][++X][//X][**X]");
	}
	else {
		tree->addV('X', "F[-X][X][+X][*X][/X]");
	}
	tree->addV('F', "FF");
	tree->iterate(iTRtime);
	nodeTree = tree->getResult(obj, shader);
	pos = glm::vec3(rand() % 600 - 300, 0.0f, rand() % 600 - 300);
	nodeTree->M = glm::translate(glm::mat4(1.0f), pos) * glm::rotate(glm::mat4(1.0f), glm::pi<float>() / (rand() % 6), glm::vec3(0.0f, 1.0f, 0.0f));
	//nodeTree->M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 4, glm::vec3(0.0f, 1.0f, 0.0f));
	this->obj = obj;
	this->shader = shader;
}



LTree::~LTree()
{
	delete(tree);
	delete(nodeTree);
}

void LTree::draw()
{
	nodeTree->draw(glm::mat4(1.0f));
}

void LTree::updateHeight(float height)
{
	nodeTree->M= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, height - 7.0f, 0.0f)) * glm::translate(glm::mat4(1.0f), pos) * glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 4, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 LTree::getPos()
{
	return glm::vec3(nodeTree->M[3][0], 0.0f, nodeTree->M[3][2]);
}

void LTree::updateTree()
{
	delete(nodeTree);
	nodeTree = tree->getResult(obj, shader);
	nodeTree->M = glm::translate(glm::mat4(1.0f), pos) * glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 4, glm::vec3(0.0f, 1.0f, 0.0f));
}
