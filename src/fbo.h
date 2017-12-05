#ifndef _FBO_H_
#define _FBO_H_
#define GLFW_INCLUDE_GLEXT
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
using namespace std;
class SingleSampleFbo {
public:
	GLuint fbo, colormap, depthmap;
	SingleSampleFbo(int, int);
	~SingleSampleFbo();
	void refresh(int, int);
private:
	void initFunc(int width, int height);
	void deleteFunc();
};
#endif
