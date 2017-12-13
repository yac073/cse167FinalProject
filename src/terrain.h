#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include "math.h"
#include "ppm.h"
#include "Window.h"
#include <math.h>
#include "Lightz.h"
#include "PerlinNoise.h"

class Terrain
{
public:
	int type;
	int random;

	ppm * heightmap;

	float x, y;

	float * vertices, * normals, * textureCoords;
	int * indices;

	GLuint VBO_vertices, VBO_normals, VBO_textureCoords, VAO, EBO;
	GLuint uProjection, uModel, uView;

	GLuint texture1, texture2, texture3, texture4, blend_texture, blend_texture2, blend_texture3, blend_texture4, blend_texture5;

	Terrain(int, int);
	~Terrain();
	void generateTerrain_plain();
	void setModernOpenGL_plain();

	void loadTexture();
	void getRand();
	void draw(GLint shaderProgram);

	static unsigned int length;

	Terrain(const char *filepath);
	void generateTerrain_random();
	void setModernOpenGL_random();
	float getHeight(int x, int y);
	glm::vec3 calcuclateNormal(int x, int y);

	int seed1, seed2;

	void regenerate_random();
	float getHeight_random(int x, int y);
	glm::vec3 calcuclateNormal_random(int x, int y);
	float getInterpolatedNoise(float x, float y);
	float interpolate(float a, float b, float);
	float getSmoothNoise(int x, int y);
	float getNoise(int x, int y);
	float getMapHeight(float x, float y);
};

#endif