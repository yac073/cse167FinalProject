#include "terrain.h"
#include "ppm.h"

using namespace std;
using namespace glm;

#define SIZE 800
#define VERTEX_COUNT 128
#define SINSAMPLE 40

#define MAX_HEIGHT 80
#define MAX_PIXEL_COLOR 256 * 256 *256

float amplitude[SINSAMPLE * 2];
float map[1026][1026];
float realMap[1024][1024];


void Terrain::generateTerrain_plain() {
	int count = VERTEX_COUNT * VERTEX_COUNT;
	vertices = new float[count * 3];
	normals = new float[count * 3];
	textureCoords = new float[count * 2];
	indices = new int[6 * (VERTEX_COUNT - 1)*(VERTEX_COUNT - 1)];
	int vertexPointer = 0;
	for (int i = 0; i<VERTEX_COUNT; i++) {
		for (int j = 0; j<VERTEX_COUNT; j++) {
			vertices[vertexPointer * 3] = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
			vertices[vertexPointer * 3 + 1] = 0;
			vertices[vertexPointer * 3 + 2] = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;
			normals[vertexPointer * 3] = 0;
			normals[vertexPointer * 3 + 1] = 1;
			normals[vertexPointer * 3 + 2] = 0;
			textureCoords[vertexPointer * 2] = (float)j / ((float)VERTEX_COUNT - 1);
			textureCoords[vertexPointer * 2 + 1] = (float)i / ((float)VERTEX_COUNT - 1);
			vertexPointer++;
		}
	}
	int pointer = 0;
	for (int gz = 0; gz<VERTEX_COUNT - 1; gz++) {
		for (int gx = 0; gx<VERTEX_COUNT - 1; gx++) {
			int topLeft = (gz*VERTEX_COUNT) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1)*VERTEX_COUNT) + gx;
			int bottomRight = bottomLeft + 1;
			indices[pointer++] = topLeft;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = topRight;
			indices[pointer++] = topRight;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = bottomRight;
		}
	}
}

Terrain::Terrain(int x, int y)
{
	type = 0;
	this->x = (float)x;
	this->y = (float)y;

	generateTerrain_plain();
	setModernOpenGL_plain();
	loadTexture();
}

Terrain::~Terrain()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO_vertices);
	glDeleteBuffers(1, &VBO_normals);
	glDeleteBuffers(1, &VBO_textureCoords);
	glDeleteBuffers(1, &EBO);
	delete(heightmap);
}

void Terrain::setModernOpenGL_plain()
{
	// Create array object and buffers.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_vertices);
	glGenBuffers(1, &VBO_normals);
	glGenBuffers(1, &VBO_textureCoords);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object (VAO)
	glBindVertexArray(VAO);

	// Bind VBO_vertices to it as a GL_ARRAY_BUFFER.
	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
	glBufferData(GL_ARRAY_BUFFER, VERTEX_COUNT * VERTEX_COUNT * 3 * sizeof(*vertices), vertices, GL_STATIC_DRAW);
	// Enable the usage of layout location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Bind VBO_normals to it as a GL_ARRAY_BUFFER.
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, VERTEX_COUNT * VERTEX_COUNT * 3 * sizeof(*normals), normals, GL_STATIC_DRAW);
	// Enable the usage of layout location 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_textureCoords);
	glBufferData(GL_ARRAY_BUFFER, VERTEX_COUNT * VERTEX_COUNT * 2 * sizeof(*textureCoords), textureCoords, GL_STATIC_DRAW);
	//Enable the usage of layout location 2
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

	// Bind the Element Buffer Object (EBO)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * (VERTEX_COUNT - 1)*(VERTEX_COUNT - 1) * sizeof(*indices), indices, GL_STATIC_DRAW);

	// Unbind to avoid unexpected changes
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Terrain::loadTexture()
{
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
											// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	int width, height, nrChannels;
	unsigned char* image = stbi_load("../res/grassy2.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
										   // Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	image = stbi_load("../res/mud_preview.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
										   // Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	image = stbi_load("../res/grassFlowers.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
											// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	image = stbi_load("../res/path.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glGenTextures(1, &blend_texture);
	glBindTexture(GL_TEXTURE_2D, blend_texture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
											// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	image = stbi_load("../res/blendmap.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glGenTextures(1, &blend_texture2);
	glBindTexture(GL_TEXTURE_2D, blend_texture2); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
												 // Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	image = stbi_load("../res/blendmap2.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glGenTextures(1, &blend_texture3);
	glBindTexture(GL_TEXTURE_2D, blend_texture3); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
												 // Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	image = stbi_load("../res/blendmap3.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glGenTextures(1, &blend_texture4);
	glBindTexture(GL_TEXTURE_2D, blend_texture4); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
												 // Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	image = stbi_load("../res/blendmap4.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glGenTextures(1, &blend_texture5);
	glBindTexture(GL_TEXTURE_2D, blend_texture5); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
												 // Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	image = stbi_load("../res/blendmap5.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
}

void Terrain::getRand()
{
	for (int i = 0; i < SINSAMPLE * 2; i++) {
		int ran = rand() % 100;
		amplitude[i] = (((float)ran) / 100.0f) / (pow(1.2, i % SINSAMPLE));
	}
}

void Terrain::draw(GLint shaderProgram)
{
	glUseProgram(shaderProgram);
	if (type == 0) {
		mat4 toWorld = translate(mat4(1.0f), vec3(x, y, -400.0f));
		// forward the projection, view, and model matrices to the shader programs
		// Get the location of the uniform variables "projection" and "modelview"
		uProjection = glGetUniformLocation(shaderProgram, "projection");
		uModel = glGetUniformLocation(shaderProgram, "model");
		uView = glGetUniformLocation(shaderProgram, "view");
		// send these values to the shader program
		glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
		glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
		glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);

		// draw the object. Bind the VAO associated with it.
		glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex2"), 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture3);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex3"), 2);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex4"), 3);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, blend_texture);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex_blend"), 4);

		// Tell OpenGL to draw with triangles, number of indices, the type of the indices, and the offset
		glDrawElements(GL_TRIANGLES, 6 * (VERTEX_COUNT - 1)*(VERTEX_COUNT - 1), GL_UNSIGNED_INT, 0);
		// Unbind the VAO to avoid unexpeceted change
		glBindVertexArray(0);
	}
	else if (type == 1) {
		mat4 toWorld = translate(mat4(1.0f), vec3(-400.0f, -5.0f, -400.0f));
		// forward the projection, view, and model matrices to the shader programs
		// Get the location of the uniform variables "projection" and "modelview"
		uProjection = glGetUniformLocation(shaderProgram, "projection");
		uModel = glGetUniformLocation(shaderProgram, "model");
		uView = glGetUniformLocation(shaderProgram, "view");
		GLuint uRandom = glGetUniformLocation(shaderProgram, "random");
		// send these values to the shader program
		glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
		glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
		glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);
		glUniform1f(uRandom, random);

		DirectionLight::uLight_direction = glGetUniformLocation(shaderProgram, "light.direction");
		DirectionLight::uLight_color = glGetUniformLocation(shaderProgram, "light.color");
		glUniform3fv(DirectionLight::uLight_direction, 1, &DirectionLight::direction[0]);
		glUniform3fv(DirectionLight::uLight_color, 1, &DirectionLight::color[0]);


		// draw the object. Bind the VAO associated with it.
		glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex2"), 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture3);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex3"), 2);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex4"), 3);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, blend_texture);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex_blend"), 4);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, blend_texture2);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex_blend2"), 5);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, blend_texture3);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex_blend3"), 6);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, blend_texture4);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex_blend4"), 7);
		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, blend_texture5);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex_blend5"), 8);
		// Tell OpenGL to draw with triangles, number of indices, the type of the indices, and the offset
		glDrawElements(GL_TRIANGLES, 6 * (length - 1)*(length - 1), GL_UNSIGNED_INT, 0);
		// Unbind the VAO to avoid unexpeceted change
		glBindVertexArray(0);
	}
}



unsigned int Terrain::length;

Terrain::Terrain(const char *filepath) {
	// Create array object and buffers.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_vertices);
	glGenBuffers(1, &VBO_normals);
	glGenBuffers(1, &VBO_textureCoords);
	glGenBuffers(1, &EBO);

	type = 1;

	random = 0;

	heightmap = new ppm(filepath);

	length = heightmap->width;
	cerr << length << endl;

	generateTerrain_random();
	setModernOpenGL_random();
	loadTexture();
}

vec3 Terrain::calcuclateNormal(int x, int y)
{
	float heightL = getHeight(x, y);
	float heightR = getHeight(x+1, y);
	float heightD = getHeight(x, y-1);
	float heightU = getHeight(x, y+1);

	vec3 normal(heightL - heightR, 2.0f, heightD - heightU);
	return normalize(normal);
}

float Terrain::getHeight(int x, int y)
{
	if (x < 0 || x >= length || y < 0 || y >= length)
		return 0.0f;

	float height = heightmap->getRGB(x, y);

	height -= MAX_PIXEL_COLOR / 2.0f;
	height /= MAX_PIXEL_COLOR / 2.0f;
	height *= MAX_HEIGHT;

	//cerr << height << endl;
	return height;

	random = 0;
}

vec3 Terrain::calcuclateNormal_random(int x, int y)
{
	float heightL = map[x - 1][y];
	float heightR = map[x + 1][y];
	float heightD = map[x][y - 1];
	float heightU = map[x][y + 1];

	vec3 normal(heightL - heightR, 2.0f, heightD - heightU);
	return normalize(normal);
}

float Terrain::getHeight_random(int x, int y)
{

	float total = 0;
	for (int i = 0; i < SINSAMPLE; i++) {
		total += (amplitude[i] * sinf((i * 2 + 1) *x * glm::pi<float>() / 1024.0f) + amplitude[i + SINSAMPLE] * sinf((i * 2 + 1) * y * glm::pi<float>() / 1024.0f));

		/*if (i % 2 == 0) {
			total += (amplitude[i] * sinf((i * 2 + 1) *x * glm::pi<float>() / 1024.0f) + amplitude[i + SINSAMPLE] * sinf((i * 2 + 1) * y * glm::pi<float>() / 1024.0f));
			//total += (sinf((i * 2 + 1) *x * glm::pi<float>() / 1024.0f) + sinf((i * 2 + 1) * y * glm::pi<float>() / 1024.0f));
		}
		else {
			total += (amplitude[i] * sinf((i * 2 + 1) *x * glm::pi<float>() / 1024.0f) + amplitude[i + SINSAMPLE] * sinf((i * 2 + 1) * y * glm::pi<float>() / 1024.0f));
			//total -= (sinf((i * 2 + 1) *x * glm::pi<float>() / 1024.0f) + sinf((i * 2 + 1) * y * glm::pi<float>() / 1024.0f));
		}*/

	}
	float result = total * MAX_HEIGHT;
	float fresult = sqrt(abs(result)) * sqrt(MAX_HEIGHT);
	if (result < 0) { fresult *= -1; }	
	return total * MAX_HEIGHT;
}

float Terrain::getInterpolatedNoise(float x, float y)
{
	int intX = (int)x;
	int intY = (int)y;
	float fracX = x - intX;
	float fracY = y - intY;

	float v1 = getSmoothNoise(intX, intY);
	float v2 = getSmoothNoise(intX + 1, intY);
	float v3 = getSmoothNoise(intX, intY + 1);
	float v4 = getSmoothNoise(intX + 1, intY + 1);

	float i1 = interpolate(v1, v2, fracX);
	float i2 = interpolate(v3, v4, fracX);

	return interpolate(i1, i2, fracY);
}

float Terrain::interpolate(float a, float b, float blend)
{
	float theta = blend * pi<float>();
	float f = (float)(1.0f - cosf(theta)) * 0.5f;
	return a * (1.0f - f) + b * f;
}

float Terrain::getSmoothNoise(int x, int y)
{
	float corners = (getNoise(x-1, y-1) + getNoise(x+1, y-1) + getNoise(x-1, y+1) + getNoise(x+1, +1)) / 16.0f;
	float sides = (getNoise(x-1, y) + getNoise(x+1, y) + getNoise(x, y-1) + getNoise(x, y+1)) / 8.0f;
	float center = getNoise(x, y) / 4.0f;
	return corners + sides + center;
}

float Terrain::getNoise(int x, int y)
{
	srand(x * 1234 + y * 8617 + seed1 * 7162 + seed2 * 5326);
	float temp = (float)rand();
	return (temp / 32767) * 2.0f - 1.0f;
}

float Terrain::getMapHeight(float x, float y)
{
	x = (x + 400.0f) / SIZE * 1024; 
	y = (y + 400.0f) / SIZE * 1024;	
	return (realMap[(int)floor(x)][(int)floor(y)] + realMap[(int)ceil(x)][(int)ceil(y)]) / 2.0f;
}

void Terrain::regenerate_random()
{
	srand((unsigned)time(0));
	random = rand() % 5;
	seed1 = rand();
	seed2 = rand();
	int count = length * length;
	vertices = new float[count * 3];
	normals = new float[count * 3];
	textureCoords = new float[count * 2];
	indices = new int[6 * (length - 1)*(length - 1)];
	int vertexPointer = 0;
	getRand();
	PerlinNoise* noise = new PerlinNoise(rand());
	for (unsigned int i = 0; i < length + 2; i++) {
		for (unsigned int j = 0; j < length + 2; j++) {
			double n = noise->OctavePerlin(i / 102.6f, j / 102.6f, 0.8, 5, 0.5);
			//double n = (double)i / 1024 + (double)j / 1024;
			map[j][i] = (float)n * MAX_HEIGHT * 3 - 2 * MAX_HEIGHT;
			if (i != 0 && i != length + 1 && j != 0 && j != length + 1) {
				vertices[vertexPointer * 3] = (float)j / ((float)length - 1) * SIZE;
				vertices[vertexPointer * 3 + 1] = map[j][i];
				realMap[j - 1][i - 1] = map[j][i];
				vertices[vertexPointer * 3 + 2] = (float)i / ((float)length - 1) * SIZE;
				textureCoords[vertexPointer * 2] = (float)j / ((float)length - 1);
				textureCoords[vertexPointer * 2 + 1] = (float)i / ((float)length - 1);
				vertexPointer++;
			}
		}
	}
	vertexPointer = 0;
	for (unsigned int i = 0; i < length + 2; i++) {
		for (unsigned int j = 0; j < length + 2; j++) {
			if (i != 0 && i != length - 1 && j != 0 && j != length - 1) {
				vec3 normal = calcuclateNormal_random(j, i);
				normals[vertexPointer * 3] = normal.x;
				normals[vertexPointer * 3 + 1] = normal.y;
				normals[vertexPointer * 3 + 2] = normal.z;
				vertexPointer++;
			}
		}
	}
	int pointer = 0;
	for (unsigned int gz = 0; gz<length - 1; gz++) {
		for (unsigned int gx = 0; gx<length - 1; gx++) {
			int topLeft = (gz*length) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1)*length) + gx;
			int bottomRight = bottomLeft + 1;
			indices[pointer++] = topLeft;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = topRight;
			indices[pointer++] = topRight;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = bottomRight;
		}
	}

	setModernOpenGL_random();
	delete(noise);
}

void Terrain::generateTerrain_random() {
	int count = length * length;
	vertices = new float[count * 3];
	normals = new float[count * 3];
	textureCoords = new float[count * 2];
	indices = new int[6 * (length - 1)*(length - 1)];
	int vertexPointer = 0;
	for (unsigned int i = 0; i<length; i++) {
		for (unsigned int j = 0; j<length; j++) {
			vertices[vertexPointer * 3] = (float)j / ((float)length - 1) * SIZE;
			vertices[vertexPointer * 3 + 1] = getHeight(j, i);
			vertices[vertexPointer * 3 + 2] = (float)i / ((float)length - 1) * SIZE;
			vec3 normal = calcuclateNormal(j, i);
			normals[vertexPointer * 3] = normal.x;
			normals[vertexPointer * 3 + 1] = normal.y;
			normals[vertexPointer * 3 + 2] = normal.z;
			textureCoords[vertexPointer * 2] = (float)j / ((float)length - 1);
			textureCoords[vertexPointer * 2 + 1] = (float)i / ((float)length - 1);
			vertexPointer++;
		}
	}
	int pointer = 0;
	for (unsigned int gz = 0; gz<length - 1; gz++) {
		for (unsigned int gx = 0; gx<length - 1; gx++) {
			int topLeft = (gz*length) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1)*length) + gx;
			int bottomRight = bottomLeft + 1;
			indices[pointer++] = topLeft;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = topRight;
			indices[pointer++] = topRight;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = bottomRight;
		}
	}
}

void Terrain::setModernOpenGL_random()
{
	// Bind the Vertex Array Object (VAO)
	glBindVertexArray(VAO);

	// Bind VBO_vertices to it as a GL_ARRAY_BUFFER.
	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
	glBufferData(GL_ARRAY_BUFFER, length * length * 3 * sizeof(*vertices), vertices, GL_STATIC_DRAW);
	// Enable the usage of layout location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Bind VBO_normals to it as a GL_ARRAY_BUFFER.
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, length * length * 3 * sizeof(*normals), normals, GL_STATIC_DRAW);
	// Enable the usage of layout location 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_textureCoords);
	glBufferData(GL_ARRAY_BUFFER, length * length * 2 * sizeof(*textureCoords), textureCoords, GL_STATIC_DRAW);
	//Enable the usage of layout location 2
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

	// Bind the Element Buffer Object (EBO)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * (length - 1)*(length - 1) * sizeof(*indices), indices, GL_STATIC_DRAW);

	// Unbind to avoid unexpected changes
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

