#include "OBJObjectz.h"
#include <iostream>
#include "glm/ext.hpp"
#include "Window.h"
#include <vector>


GLuint OBJObjectz::bodyTexture;
GLuint OBJObjectz::legTexture;

using namespace std;
using namespace glm;

OBJObjectz::OBJObjectz(const char *filepath)
{
	parse(filepath);
	loadArrays();
	setModernOpenGL();
}

void OBJObjectz::parse(const char *filepath)
{
	FILE* fp;     // file pointer
	float x, y, z;  // vertex coordinates
	float r, g, b;  // vertex color
	int c1, c2;    // characters read from file

	errno_t result = fopen_s(&fp, filepath, "rb");  // make the file name configurable so you can load other files
	if (result) { cerr << "error loading file" << endl; exit(-1); }  // just in case the file can't be found or is corrupt
	while (true) {
		if ((c1 = fgetc(fp)) != EOF) {
			if (c1 == 'v') {
				c2 = fgetc(fp);
				if (c2 == ' ') {
					fscanf_s(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
					vertices.push_back(vec3(x, y, z));
				}
				else if (c2 == 'n') {
					fscanf_s(fp, "%f %f %f", &x, &y, &z);
					normals.push_back(normalize(vec3(x, y, z)));
				}
			}
			else if (c1 == 'f') {
				c2 = fgetc(fp);
				if (c2 == ' ') {
					int a, b, c, d, e, f;
					fscanf_s(fp, "%d // %d %d // %d %d // %d", &a, &b, &c, &d, &e, &f);
					vector<pair<int, int>> temp;
					temp.push_back(pair<int, int>(a, b));
					temp.push_back(pair<int, int>(c, d));
					temp.push_back(pair<int, int>(e, f));
					faces.push_back(temp);
				}
			}
		}
		else
			break;
	}
	faces.pop_back();
	fclose(fp);
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
}

void OBJObjectz::loadArrays()
{
	int count = 0;
	// handle the vertex array
	GLvertices = new GLfloat[vertices.size() * 3];
	for (unsigned int i = 0; i < vertices.size(); ++i) {
		GLvertices[count] = vertices[i].x;
		GLvertices[count + 1] = vertices[i].y;
		GLvertices[count + 2] = vertices[i].z;
		count += 3;
	}

	count = 0;
	// handle the vertex array
	GLfaces = new GLuint[faces.size() * 3];
	for (unsigned int i = 0; i < faces.size(); ++i) {
		GLfaces[count] = faces[i][0].first - 1;
		GLfaces[count + 1] = faces[i][1].first - 1;
		GLfaces[count + 2] = faces[i][2].first - 1;
		count += 3;
	}

	count = 0;
	// handle the vertex array
	GLnormals = new GLfloat[normals.size() * 3];
	for (unsigned int i = 0; i < normals.size(); ++i) {
		GLnormals[count] = normals[i].x;
		GLnormals[count + 1] = normals[i].y;
		GLnormals[count + 2] = normals[i].z;
		count += 3;
	}
}

void OBJObjectz::setModernOpenGL()
{
	// Create array object and buffers.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_vertices);
	glGenBuffers(1, &VBO_normals);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object (VAO)
	glBindVertexArray(VAO);

	// Bind VBO_vertices to it as a GL_ARRAY_BUFFER.
	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(*GLvertices), GLvertices, GL_STATIC_DRAW);
	// Enable the usage of layout location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Bind VBO_normals to it as a GL_ARRAY_BUFFER.
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * 3 * sizeof(*GLnormals), GLnormals, GL_STATIC_DRAW);
	// Enable the usage of layout location 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Bind the Element Buffer Object (EBO)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * 3 * sizeof(*GLfaces), GLfaces, GL_STATIC_DRAW);

	// Unbind to avoid unexpected changes
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

OBJObjectz::~OBJObjectz()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO_vertices);
	glDeleteBuffers(1, &VBO_normals);
	glDeleteBuffers(1, &EBO);
	delete []GLvertices;
	delete []GLnormals;
	delete []GLfaces;
}

void OBJObjectz::loadSpongeBobTextures() {

	vector <const GLchar *> body;
	body.push_back("../res/bob_right.jpg");
	body.push_back("../res/bob_left.jpg");
	body.push_back("../res/bob_top.jpg");
	body.push_back("../res/bob_bottom.jpg");
	body.push_back("../res/bob_front.jpg");
	body.push_back("../res/bob_back.jpg");

	GLuint ID;
	int width, height, nrChannels;
	unsigned char* tdata;

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

	for (GLuint i = 0; i < body.size(); i++)
	{
		tdata = stbi_load(body[i], &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
		stbi_image_free(tdata);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	bodyTexture = ID;

	// ---------------- second part -------------
	vector <const GLchar *> leg;
	leg.push_back("../res/bob_leg.jpg");
	leg.push_back("../res/bob_leg.jpg");
	leg.push_back("../res/bob_leg.jpg");
	leg.push_back("../res/bob_leg.jpg");
	leg.push_back("../res/bob_leg.jpg");
	leg.push_back("../res/bob_leg.jpg");

	GLuint textureID_2;

	glGenTextures(1, &textureID_2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID_2);

	for (GLuint i = 0; i < leg.size(); i++)
	{
		tdata = stbi_load(leg[i], &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
		stbi_image_free(tdata);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	legTexture = textureID_2;
}


void OBJObjectz::draw(mat4 toWorld, GLint shaderProgram, bool body_or_leg)
{
	glUseProgram(shaderProgram);
	// forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModel = glGetUniformLocation(shaderProgram, "model");
	uView = glGetUniformLocation(shaderProgram, "view");
	// send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);

	DirectionLight::uLight_direction = glGetUniformLocation(shaderProgram, "light.direction");
	DirectionLight::uLight_color = glGetUniformLocation(shaderProgram, "light.color");
	glUniform3fv(DirectionLight::uLight_direction, 1, &DirectionLight::direction[0]);
	glUniform3fv(DirectionLight::uLight_color, 1, &DirectionLight::color[0]);

	// draw the object. Bind the VAO associated with it.
	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	if (body_or_leg) {
		glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, bodyTexture);
	}
	else if (!body_or_leg) {
		glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, legTexture);
	}
	// Tell OpenGL to draw with triangles, number of indices, the type of the indices, and the offset
	glDrawElements(GL_TRIANGLES, (GLsizei)faces.size() * 3, GL_UNSIGNED_INT, 0);
	// Unbind the VAO to avoid unexpeceted change
	glBindVertexArray(0);
}

void OBJObjectz::toCenterAndStandardSize(float &s, float &x, float &y, float &z)
{
	// declare variables
	float x_min, x_max, y_min, y_max, z_min, z_max, x_range, y_range, z_range, largest_range;
	x_min = y_min = z_min = 0.0f;
	x_max = y_max = z_max = 0.0f;
	// find the max and min value of x, y, z coordinates
	for (unsigned int i = 0; i < vertices.size(); ++i)
	{
		if (vertices[i].x < x_min)
			x_min = vertices[i].x;
		if (vertices[i].x > x_max)
			x_max = vertices[i].x;
		if (vertices[i].y < y_min)
			y_min = vertices[i].y;
		if (vertices[i].y > y_max)
			y_max = vertices[i].y;
		if (vertices[i].z < z_min)
			z_min = vertices[i].z;
		if (vertices[i].z > z_max)
			z_max = vertices[i].z;
	}
	// get the ranges
	x_range = abs(x_max - x_min);
	y_range = abs(y_max - y_min);
	z_range = abs(z_max - z_min);
	largest_range = glm::max(x_range, glm::max(y_range, z_range));
	// update the fields
	s = 2 / largest_range;
	x = -(x_max + x_min) / 2.0f * s;
	y = -(y_max + y_min) / 2.0f * s;
	z = -(z_max + z_min) / 2.0f * s;
}

void OBJObjectz::toCenterAndStandardSize(float &s, float &x, float &y, float &z, vec3 &center_origin, float &radius_origin)
{
	// declare variables
	float x_min, x_max, y_min, y_max, z_min, z_max, x_range, y_range, z_range, largest_range;
	x_min = y_min = z_min = 0.0f;
	x_max = y_max = z_max = 0.0f;
	// find the max and min value of x, y, z coordinates
	for (unsigned int i = 0; i < vertices.size(); ++i)
	{
		if (vertices[i].x < x_min)
			x_min = vertices[i].x;
		if (vertices[i].x > x_max)
			x_max = vertices[i].x;
		if (vertices[i].y < y_min)
			y_min = vertices[i].y;
		if (vertices[i].y > y_max)
			y_max = vertices[i].y;
		if (vertices[i].z < z_min)
			z_min = vertices[i].z;
		if (vertices[i].z > z_max)
			z_max = vertices[i].z;
	}
	// get the ranges
	x_range = abs(x_max - x_min);
	y_range = abs(y_max - y_min);
	z_range = abs(z_max - z_min);
	largest_range = glm::max(x_range, glm::max(y_range, z_range));
	// update the fields
	center_origin = vec3((x_max + x_min) / 2.0f, (y_max + y_min) / 2.0f, (z_max + z_min) / 2.0f);
	s = 2 / largest_range;
	radius_origin = largest_range / 2;
	x = -(x_max + x_min) / 2.0f * s;
	y = -(y_max + y_min) / 2.0f * s;
	z = -(z_max + z_min) / 2.0f * s;
}
