#ifndef _MAIN_H_
#define _MAIN_H_

#define GLFW_INCLUDE_GLEXT
#define STB_IMAGE_IMPLEMENTATION
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include "window.h"
#include "GL/glut.h"
#include <iostream>
using namespace irrklang;
#endif
