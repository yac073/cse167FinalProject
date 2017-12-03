#include "Material.h"
Material::Material(int Type){
	switch (Type)
	{
	case 0:
		Shininess = 5.0f;
		Diffuse = glm::vec3(0.0f);
		Ambient = glm::vec3(.5f);
		Specular = glm::vec3(.5f, .5f, .5f);
		Color = glm::vec3(1.0f, 1.0f, 0.0f);
		break;
	case 1:
		Diffuse = glm::vec3(1.0f,1.0f,1.0f);
		Ambient = glm::vec3(0.0f);
		Shininess = 0;
		Color = glm::vec3(0.5f, 0.0f, 0.5f);
		Specular = glm::vec3(0.0f);
		break;
	case 2:
		Diffuse = glm::vec3(1.0f);
		Ambient = glm::vec3(0.3f);
		Specular = glm::vec3(1.0f);
		Color = glm::vec3(0.7f, 0.7f, 0.7f);
		Shininess = 5.0f;
		break;
	case 3:
		Shininess = 1.0f;
		Diffuse = glm::vec3(0.0f);
		Ambient = glm::vec3(1.0f);
		Specular = glm::vec3(0.0f, 0.0f, 0.0f);
		Color = glm::vec3(1.0f, 1.0f, 1.0f);
		break;
	default:
		break;
	}
}