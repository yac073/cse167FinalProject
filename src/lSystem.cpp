#include "lSystem.h"

void lSystem::buildChild(OBJObject * o, glm::vec3 size, GLint shader, glm::vec3 dir, Transform* childgroup)
{
	glm::vec3 origin(0.0f, 0.0f, 0.0f);
	while (index2 < w.size()) {
		if (w[index2] == '[') {
			auto child = new Transform();
			child->M = glm::translate(glm::mat4(1.0f), glm::vec3(origin));
			index2++;
			buildChild(o, size * 0.95f, shader, dir * 0.95f , child);
			childgroup->addChild(child);
		}
		else if (w[index2] == ']') {
			index2++;
			return;
		}
		else if (w[index2] == '+' && (rand() % 10 > randNum)) {
			dir = glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 6, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(dir, 1.0f);
			index2++;
		}
		else if (w[index2] == '-' && (rand() % 10 > randNum)) {
			dir = glm::rotate(glm::mat4(1.0f), -glm::pi<float>() / 6, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(dir, 1.0f);
			index2++;
		}
		else if (w[index2] == '*' && (rand() % 10 > randNum)) {
			dir = glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 6, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(dir, 1.0f);
			index2++;
		}
		else if (w[index2] == '/' && (rand() % 10 > randNum)) {
			dir = glm::rotate(glm::mat4(1.0f), -glm::pi<float>() / 6, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(dir, 1.0f);
			index2++;
		}
		else {
			dir *= 0.95f;
			origin += dir;
			auto trans = new Transform();
			auto child = new Geometry(o, shader, w[index2]);
			trans->M = glm::translate(glm::mat4(1.0f), glm::vec3(origin)) * glm::scale(glm::mat4(1.0f), glm::vec3(size));
			trans->addChild(child);
			childgroup->addChild(trans);
			index2++;
		}
	}
}

lSystem::lSystem()
{
	V = new vector<pair<char, string>>();
	S = new vector<pair<char, int>>();
}

lSystem::~lSystem()
{
	delete(V);
	delete(S);
}

void lSystem::addV(char c, string s)
{
	V->push_back(pair<char, string>(c,s));
}

void lSystem::addS(char c, int i)
{
	S->push_back(pair<char, int>(c, i));
}

string lSystem::getString()
{
	return w;
}

void lSystem::setString(string s)
{
	w = string(s);
}

void lSystem::iterate(int num)
{
	for (int n = 0; n < num; n++) {
		char newW[1000000];
		int j = 0;
		for (int i = 0; i < w.size(); i++) {
			char index = w[i];
			if (w[i] == '[') {
				newW[j] = '['; j++;
			}
			else if (w[i] == ']') {
				newW[j] = ']'; j++;
			}
			else if (w[i] == '+') {
				newW[j] = '+'; j++;
			}
			else if (w[i] == '-') {
				newW[j] = '-'; j++;
			}
			else if (w[i] == '*') {
				newW[j] = '*'; j++;
			}
			else if (w[i] == '/') {
				newW[j] = '/'; j++;
			}
			else {
				for each(auto var in *V) {
					if (var.first == index) {
						for (int k = 0; k < var.second.size(); k++) {
							newW[j] = var.second[k];
							j++;
						}
					}
				}
			}
		}
		newW[j] = '\0';
		w = string(newW);
	}
}

Transform * lSystem::getResult(OBJObject * o, GLint shader)
{
	srand(rand());
	auto result = new Transform();
	glm::vec3 dir(0.0f, 4.0f, 0.0f);
	glm::vec3 size(2.0f, 2.0f, 2.0f);
	glm::vec3 origin(0.0f, 0.0f, 0.0f);
	index2 = 0;
	while (index2 < w.size()) {
		if (w[index2] == '[') {
			auto child = new Transform();
			child->M = glm::translate(glm::mat4(1.0f), glm::vec3(origin));
			index2++;
			buildChild(o, size * 0.95f, shader, dir * 0.95f, child);
			result->addChild(child);
		}
		else if (w[index2] == ']') {
			index2++;
		}
		else if (w[index2] == '+' && (rand() % 10 > randNum)) {
			dir = glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 6, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(dir, 1.0f);
			index2++;
		}
		else if (w[index2] == '-' && (rand() % 10 > randNum)) {
			dir = glm::rotate(glm::mat4(1.0f), -glm::pi<float>() / 6, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(dir, 1.0f);
			index2++;
		}
		else if (w[index2] == '*' && (rand() % 10 > randNum)) {
			dir = glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 6, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(dir, 1.0f);
			index2++;
		}
		else if (w[index2] == '/' && (rand() % 10 > randNum)) {
			dir = glm::rotate(glm::mat4(1.0f), -glm::pi<float>() / 6, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(dir, 1.0f);
			index2++;
		}
		else {
			dir *= 0.95f;
			size *= 0.95f;
			origin += dir;
			auto trans = new Transform();
			auto child = new Geometry(o, shader, w[index2]);
			trans->M = glm::translate(glm::mat4(1.0f), glm::vec3(origin)) * glm::scale(glm::mat4(1.0f), glm::vec3(size));
			trans->addChild(child);
			result->addChild(trans);
			index2++;
		}		
	}
	return result;
}
