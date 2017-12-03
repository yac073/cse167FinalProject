#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
class Material {
public:
	glm::vec3 Diffuse;
	glm::vec3 Ambient;
	glm::vec3 Specular;
	glm::vec3 Color;
	float Shininess;
	int Type;
	Material::Material(int Type);
};

