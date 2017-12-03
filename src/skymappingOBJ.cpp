#define _CRT_SECURE_NO_WARNINGS
#include "skymappingOBJ.h"
#include "Window.h"

SkymappingOBJ::SkymappingOBJ(const char *filepath)
{
	toWorld = glm::mat4(1.0f);
	transform = glm::mat4(1.0f);
	useNormal = false;
	parse(filepath);
	skytexture = loadCubemap(faces);
	mass = 1.0f;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO_N);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_N);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

SkymappingOBJ::~SkymappingOBJ()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO_N);
	glDeleteBuffers(1, &EBO);
}

void SkymappingOBJ::parse(const char *filepath)
{
	FILE* fp;
	float x, y, z, r, g, b;
	unsigned int x11, x12, x21, x22, x31, x32;
	int c1, c2;
	zMin = std::numeric_limits<float>::max();
	zMax = std::numeric_limits<float>::min();
	yMin = std::numeric_limits<float>::max();
	yMax = std::numeric_limits<float>::min();
	xMin = std::numeric_limits<float>::max();
	xMax = std::numeric_limits<float>::min();
	fp = fopen(filepath, "rb");
	if (fp != NULL) {
		while ((c1 = fgetc(fp)) != EOF) {
			c2 = fgetc(fp);
			std::string s;
			if (c1 == '#') {
 				fscanf(fp, "%[^\n]\n", s);
			}
			else if (c1 == 'v' && c2 == ' ') {
				fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
				vertices.push_back(glm::vec3(x, y, z));
				if (z < zMin) { zMin = z; }
				if (z > zMax) { zMax = z; }
				if (x < xMin) { xMin = x; }
				if (x > xMax) { xMax = x; }
				if (y < yMin) { yMin = y; }
				if (y > yMax) { yMax = y; }
			}
			else if (c1 == 'v' && c2 == 'n') {
				fscanf(fp, "%f %f %f", &x, &y, &z);
				normals.push_back(glm::normalize(glm::vec3(x, y, z)));
			}
			else if (c1 == 'f') {
				fscanf(fp, "%u//%u %u//%u %u//%u", &x11, &x12, &x21, &x22, &x31, &x32);
				indices.push_back(x11 - 1);
				indices.push_back(x21 - 1);
				indices.push_back(x31 - 1);
			}
		}
		fclose(fp);
	}
	toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(-(xMin + xMax) / 2, -(yMin + yMax) / 2, -(zMin + zMax) / 2)) * toWorld;
}

void SkymappingOBJ::draw(GLuint shaderProgram)
{	
	glUseProgram(shaderProgram);
	if (shaderProgram != -1) {
		glm::mat4 view = Window::V * transform;
		uProjection = glGetUniformLocation(shaderProgram, "projection");
		uView = glGetUniformLocation(shaderProgram, "view");
		uModel = glGetUniformLocation(shaderProgram, "model");
		uCampos = glGetUniformLocation(shaderProgram, "cameraPos");
		// Now send these values to the shader program
		glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
		glUniformMatrix4fv(uView, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
		glUniform3f(uCampos, Window::cam_pos.x, Window::cam_pos.y, Window::cam_pos.z);
		// Now draw the cube. We simply need to bind the VAO associated with it.
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skytexture);
		// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
		glBindVertexArray(0);
	}
}

unsigned int SkymappingOBJ::loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

void SkymappingOBJ::update(double deltaT)
{
	moveAlong(deltaT);
}

void SkymappingOBJ::moveAlong(double dt)
{
	speed = sqrtf(2 * abs(energy) / mass);
	float frictionConstant = 2.0f;
	float distance = speed * dt;	
	if (energy < 0) {
		dir = !dir;
		energy = 0;
	}
	float d = 0;
	if (dir) {
		while (d < distance) {
			d += glm::length(curvePts[index + 1] - lastPoint);
			index = (index + 1) % 1600;
			if (friction) {
				energy -= mass * d * frictionConstant;
				if (index >= 0 && index < 200) {
					energy += 3.0f;
				}
			}
		}
	}
	else {
		while (d < distance) {
			d += glm::length(curvePts[(index + 1599) % 1600] - lastPoint);
			index = (index + 1599) % 1600;
			if (friction) {
				energy -= mass * d * frictionConstant;
				if (index >= 0 && index < 200) {
					energy += 3.0f;
				}
			}
		}
	}
	transform = glm::translate(glm::mat4(1.0f), (curvePts[index] - lastPoint)) * transform;
	float dy = lastPoint.y - curvePts[index].y;
	energy += mass * 9.8f * dy;
	facingDir = curvePts[index] - lastPoint;
	lastPoint = curvePts[index];
}

glm::mat4 SkymappingOBJ::getMatrix()
{
	return this->transform;
}

void SkymappingOBJ::setMatrix(glm::mat4 matrix)
{
	this->transform = matrix;
}

