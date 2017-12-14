#define _CRT_SECURE_NO_WARNINGS
#include "OBJObject.h"
#include "Window.h"

GLuint OBJObject::treeTexture;
GLuint OBJObject::leaveTexture;

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
	toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f / (xMax - xMin), 2.0f / (yMax - yMin), 2.0f / (zMax - zMin))) * glm::translate(glm::mat4(1.0f), glm::vec3(-(xMin + xMax) / 2, -(yMin + yMax) / 2, -(zMin + zMax) / 2)) * toWorld;
	cerr << vertices.size() << endl;
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
		glUniformMatrix4fv(uModel, 1, GL_FALSE, &(transform * toWorld)[0][0]);

		useNormal = true;

		DirectionLight::uLight_direction = glGetUniformLocation(shaderProgram, "light.direction");
		DirectionLight::uLight_color = glGetUniformLocation(shaderProgram, "light.color");
		glUniform3fv(DirectionLight::uLight_direction, 1, &DirectionLight::direction[0]);
		glUniform3fv(DirectionLight::uLight_color, 1, &DirectionLight::color[0]);

		GLuint uviewDirection = glGetUniformLocation(shaderProgram, "v");
		glUniform3fv(uviewDirection, 1, &Window::cam_pos[0]);

		// Now draw the cube. We simply need to bind the VAO associated with it.
		glBindVertexArray(VAO);
		glUniform1i(glGetUniformLocation(shaderProgram, "isInter"), isInter ? 1 : 0);
		glUniform1i(glGetUniformLocation(shaderProgram, "id"), id);
		glUniform1f(glGetUniformLocation(shaderProgram, "focal"), Window::focal);

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, treeTexture);

		glActiveTexture(GL_TEXTURE1);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex2"), 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, leaveTexture);

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

void OBJObject::loadTextures() {

	vector <const GLchar *> tree;
	tree.push_back("../res/tree.jpg");
	tree.push_back("../res/tree.jpg");
	tree.push_back("../res/tree.jpg");
	tree.push_back("../res/tree.jpg");
	tree.push_back("../res/tree.jpg");
	tree.push_back("../res/tree.jpg");

	GLuint ID;
	int width, height, nrChannels;
	unsigned char* tdata;

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

	for (GLuint i = 0; i < tree.size(); i++)
	{
		tdata = stbi_load(tree[i], &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
		stbi_image_free(tdata);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	treeTexture = ID;

	// ---------------- second part -------------
	vector <const GLchar *> leaf;
	leaf.push_back("../res/leave.jpg");
	leaf.push_back("../res/leave.jpg");
	leaf.push_back("../res/leave.jpg");
	leaf.push_back("../res/leave.jpg");
	leaf.push_back("../res/leave.jpg");
	leaf.push_back("../res/leave.jpg");

	GLuint textureID_2;

	glGenTextures(1, &textureID_2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID_2);

	for (GLuint i = 0; i < leaf.size(); i++)
	{
		tdata = stbi_load(leaf[i], &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
		stbi_image_free(tdata);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	leaveTexture = textureID_2;
}