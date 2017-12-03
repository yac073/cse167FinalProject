#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "OBJObject.h"
#include "shader.h"
#include "Curve.h"
//#include "AntTweakBar.h"
#include "tansform.h"
#include "geometry.h"
#include <time.h>
#include "skymappingOBJ.h"
#define STBI_ASSERT(x)
#include "stb_image.h"

class Window
{
	enum MovementType {
		None = 0,
		Prepare,
		Rotate,
		Translate,
		Scroll
	};
public:
	static int width;
	static int height;
	static bool mode;
	static glm::vec3 lastPoint;
	static MovementType Movement;
	//static TwBar* myBar;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static glm::mat4 pMatrix;
	static glm::vec3 cam_pos;
	static void initialize_objects();
	static void RefreshBallPos();
	static OBJObject* currentOBJ;
	static OBJObject* currentLight;
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static glm::vec3 Window::trackBallMapping(double x, double y);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static Transform* loadControls(glm::mat4);
};

#endif
