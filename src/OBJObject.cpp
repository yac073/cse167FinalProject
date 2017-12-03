#define _CRT_SECURE_NO_WARNINGS
#include "OBJObject.h"
#include "Window.h"

OBJObject::OBJObject(const char *filepath) 
{
	toWorld = glm::mat4(1.0f);
	transform = glm::mat4(1.0f);
	useNormal = false;
	parse(filepath);

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

OBJObject::~OBJObject()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO_N);
	glDeleteBuffers(1, &EBO);
}

void OBJObject::parse(const char *filepath) 
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

void OBJObject::draw(GLuint shaderProgram, bool isAlpha, bool isInter, int id)
{	
	glUseProgram(shaderProgram);
	if (shaderProgram != -1) {
		glm::mat4 modelview = Window::V * transform * toWorld;
		// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
		// Consequently, we need to forward the projection, view, and model matrices to the shader programs
		// Get the location of the uniform variables "projection" and "modelview"
		uProjection = glGetUniformLocation(shaderProgram, "projection");
		uModelview = glGetUniformLocation(shaderProgram, "modelview");
		uModel = glGetUniformLocation(shaderProgram, "model");
		// Now send these values to the shader program
		glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
		glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
		glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);

		useNormal = true;

		//glUniform3f(glGetUniformLocation(shaderProgram, "light.Color"), light.Color.x, light.Color.y, light.Color.z);
		//glUniform3f(glGetUniformLocation(shaderProgram, "light.Direction"), light.Direction.x, light.Direction.y, light.Direction.z);
		//glUniform3f(glGetUniformLocation(shaderProgram, "light.Position"), light.Position.x, light.Position.y, light.Position.z);
		//glUniform3f(glGetUniformLocation(shaderProgram, "light.ConeCenter"), light.ConeCenter.x, light.ConeCenter.y, light.ConeCenter.z);
		//glUniform1f(glGetUniformLocation(shaderProgram, "light.Cutoff"), light.Cutoff);
		//glUniform1f(glGetUniformLocation(shaderProgram, "light.Exponent"), light.Exponent);
		//glUniform1i(glGetUniformLocation(shaderProgram, "light.Type"), light.Type);

		//glUniform3f(glGetUniformLocation(shaderProgram, "material.Diffuse"), material.Diffuse.x, material.Diffuse.y, material.Diffuse.z);
		//glUniform3f(glGetUniformLocation(shaderProgram, "material.Ambient"), material.Ambient.x, material.Ambient.y, material.Ambient.z);
		//glUniform3f(glGetUniformLocation(shaderProgram, "material.Specular"), material.Specular.x, material.Specular.y, material.Specular.z);
		//glUniform3f(glGetUniformLocation(shaderProgram, "material.Color"), material.Color.x, material.Color.y, material.Color.z);
		//glUniform1f(glGetUniformLocation(shaderProgram, "material.Shininess"), material.Shininess);
		//glUniform1i(glGetUniformLocation(shaderProgram, "material.Type"), material.Type);

		//glUniform3f(glGetUniformLocation(shaderProgram, "viewPosition"), Window::cam_pos.x, Window::cam_pos.y, Window::cam_pos.z);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glUniform1i(glGetUniformLocation(shaderProgram, "useNormal"), useNormal);
		//glUniform1f(glGetUniformLocation(shaderProgram, "transparancy"), transparancy);

		// Now draw the cube. We simply need to bind the VAO associated with it.
		glBindVertexArray(VAO);
		glUniform1i(glGetUniformLocation(shaderProgram, "isAlpha"), isAlpha? 1 : 0);
		glUniform1i(glGetUniformLocation(shaderProgram, "isInter"), isInter ? 1 : 0);
		glUniform1i(glGetUniformLocation(shaderProgram, "id"), id);
		// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
		glBindVertexArray(0);
	}
}

void OBJObject::update()
{
	spin(1.0f);
}

void OBJObject::spin(float deg)
{
	toWorld = toWorld * glm::rotate(glm::mat4(1.0f), 1.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 OBJObject::getMatrix()
{
	return this->transform;
}

void OBJObject::setMatrix(glm::mat4 matrix)
{
	this->transform = matrix;
}

