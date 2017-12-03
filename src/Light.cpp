#include "Light.h"

Light::Light(int type) {
	switch (type)
	{
	case 0:
		Color = glm::vec3(1.0f, 1.0f, 1.0f);
		Direction = glm::vec3(0.0f, 10.0f, 10.0f);
		Type = 0;
		break;
	case 1:
		Color = glm::vec3(0.5f, 0.5f, 0.0f);
		Position = glm::vec3(2.0f, 2.0f, 2.0f);
		Type = 1;
		break;
	case 2:
		Color = glm::vec3(1.0f, 0.7f, 0.6f);
		Position = glm::vec3(0.0f, 0.0f, 10.0f);
		ConeCenter = glm::vec3(0.0f, 0.0f, -10.0f);
		Direction = glm::vec3(-1.0f, 0.0f, 0.0f);
		Angle = 5.0f;
		Cutoff = glm::cos(glm::radians(Angle));
		Exponent = 20.0f;
		Type = 2;
		break;
	default:
		break;
	}
}