#ifndef OBJOBJECT_H
#define OBJOBJECT_H

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
#include "Light.h"
#include "Material.h"

class OBJObject
{
private:
std::vector<unsigned int> indices;
std::vector<glm::vec3> vertices;
std::vector<glm::vec3> normals;
public:
	OBJObject(const char* filepath);
	~OBJObject();
	float angle;
	float zMin;
	float zMax;
	float xMin, xMax, yMin, yMax;
	glm::mat4 toWorld, transform;
	bool useNormal;
	int id;


	void parse(const char* filepath);
	void draw(GLuint shaderProgram, bool, bool, int);
	void update();
	void spin(float);


	GLuint VBO, VAO, EBO, VBO_N;
	GLuint uProjection, uModelview, uModel;

	Light light = Light::Light(0);
	Material material = Material::Material(0);

	glm::mat4 getMatrix();
	void setMatrix(glm::mat4 matrix);
};

#endif