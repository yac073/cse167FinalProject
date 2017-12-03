#ifndef SKYMAPPINGOBJ_H
#define SKYMAPPINGOBJ_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class SkymappingOBJ
{
private:
std::vector<unsigned int> indices;
std::vector<glm::vec3> vertices;
std::vector<glm::vec3> normals;
public:
	SkymappingOBJ(const char* filepath);
	~SkymappingOBJ();
	float angle;
	float zMin;
	float zMax;
	float xMin, xMax, yMin, yMax;
	glm::mat4 toWorld, transform;
	std::vector<glm::vec3> curvePts;
	glm::vec3 lastPoint;
	float energy, speed, mass;
	int index;
	bool useNormal;
	int id;
	bool dir, friction;
	glm::vec3 facingDir;
	std::vector<std::string> faces
	{
		"../res/Skybox_Water222_right.jpg",
		/*lf*/		"../res/Skybox_Water222_left.jpg",
		"../res/Skybox_Water222_top.jpg",
		"../res/Skybox_Water222_base.jpg",
		/*fr*/		"../res/Skybox_Water222_front.jpg",
		"../res/Skybox_Water222_back.jpg"
	};

	void parse(const char* filepath);
	void draw(GLuint shaderProgram);
	void update(double deltaT);
	void moveAlong(double dt);


	GLuint VBO, VAO, EBO, VBO_N, skytexture;
	GLuint uProjection, uView, uModel, uCampos;

	glm::mat4 getMatrix();
	void setMatrix(glm::mat4 matrix);
	unsigned int loadCubemap(std::vector<std::string> faces);
};

#endif