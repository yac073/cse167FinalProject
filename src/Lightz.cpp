#include "Light.h"
#include <iostream>
#include "glm/ext.hpp"
#include "Window.h"

using namespace std;
using namespace glm;


vec3 DirectionLight::direction;
vec3 DirectionLight::color;
GLuint DirectionLight::uLight_direction;
GLuint DirectionLight::uLight_color;

DirectionLight::DirectionLight()
{
	// needed fields
	color = vec3(1.0f, 1.0f, 1.0f);
	direction = vec3(-1.0f, -0.0f, 0.0f);
	angle = 0.0f;
	
}


DirectionLight::~DirectionLight()
{

}

void DirectionLight::update()
{
	angle += 0.5f;
	if (angle > 360.0f) angle = 0.0f;
	//mat4 R = glm::rotate(glm::mat4(1.0f), ((angle > 180.0f)? 360.0f - angle : angle) / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
	mat4 R = glm::rotate(glm::mat4(1.0f), angle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
	direction = R * vec4(-1.0f, -0.0f, 0.0f, 1.0f);
}
