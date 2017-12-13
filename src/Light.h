#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Light{
public:
glm::vec3 Color;
glm::vec3 Direction;
glm::vec3 Position;
glm::vec3 ConeCenter;
float Angle;
float Cutoff;
float Exponent;
int Type;
Light::Light(int type);
};

#endif