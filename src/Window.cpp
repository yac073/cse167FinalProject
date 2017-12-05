#include "window.h"
const char* window_title = "CSE167FinalProject";
Cube * cube;
OBJObject* sphere;
Curve * curve;
SkymappingOBJ* obj;
//OBJObject* cubeOBJ;
Transform* controls;
GLint shaderProgram, skyboxshaderProgram, curveshaderProgram, controlshaderProgram, skymapShaderProgram, dofshaderProgram, mbshaderProgram;
int robotNum, localAngle, grandAngle;
bool culling;
float robotScale;
float fov = 45.0f;
bool rectCull = false;
int selected;
double lastUpdatetime, dTime;
bool shouldUpdate;
bool shouldAllowManu = true;
float xrot = 0, yrot = 0, dX, dY;
GLuint vaoQuad, vboQuad;
int enable8bj = 0;
bool onlyfortest = false;
SingleSampleFbo* ssFbo;
list<SingleSampleFbo*>msFbo;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../src/shader.vert"
#define FRAGMENT_SHADER_PATH "../src/shader.frag"
#define CURVE_VERTEX_SHADER_PATH "../src/curveshader.vert"
#define CURVE_FRAGMENT_SHADER_PATH "../src/curveshader.frag"
#define SKYBOX_VERTEX_SHADER_PATH "../src/skyboxshader.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "../src/skyboxshader.frag"
#define CONTROL_VERTEX_SHADER_PATH "../src/controlshader.vert"
#define CONTROL_FRAGMENT_SHADER_PATH "../src/controlshader.frag"
#define SKYMAP_VERTEX_SHADER_PATH "../src/skymapshader.vert"
#define SKYMAP_FRAGMENT_SHADER_PATH "../src/skymapshader.frag"
#define DOF_VERTEX_SHADER_PATH "../src/dofshader.vert"
#define DOF_FRAGMENT_SHADER_PATH "../src/dofshader.frag"
#define MB_VERTEX_SHADER_PATH "../src/mbshader.vert"
#define MB_FRAGMENT_SHADER_PATH "../src/mbshader.frag"
// Default camera parameters
glm::vec3 Window::cam_pos(0.0f, 0.0f, 0.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, -20.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;
bool Window::mode;
OBJObject* Window::currentOBJ = nullptr;
OBJObject* Window::currentLight = nullptr;
Window::MovementType Window::Movement = Window::MovementType::None;
glm::vec3 Window::lastPoint(0.0f);
glm::mat4 Window::pMatrix(1.0f);

glm::vec3 xBiase(1.0f, 0.0f, 0.0f);
glm::vec3 yBiase(0.0f, 1.0f, 0.0f);
glm::vec3 zBiase(0.0f, 0.0f, 1.0f);

glm::mat4 Window::P;
glm::mat4 Window::V;
int Window::enableDOF = 1;
int Window::enableMB = 0;
float Window::focal;
const int MOTION_BLUR_SAMPLE_NUM = 9;
GLfloat quadVertices[] = {
	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f,  1.0f,  1.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};

void specifyScreenVertexAttributes(GLuint shaderProgram)
{
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
}

void Window::initialize_objects()
{	
	cube = new Cube();
	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	skyboxshaderProgram = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);
	curveshaderProgram = LoadShaders(CURVE_VERTEX_SHADER_PATH, CURVE_FRAGMENT_SHADER_PATH);
	controlshaderProgram = LoadShaders(CONTROL_VERTEX_SHADER_PATH, CONTROL_FRAGMENT_SHADER_PATH);
	skymapShaderProgram = LoadShaders(SKYMAP_VERTEX_SHADER_PATH, SKYMAP_FRAGMENT_SHADER_PATH);
	dofshaderProgram = LoadShaders(DOF_VERTEX_SHADER_PATH, DOF_FRAGMENT_SHADER_PATH);
	mbshaderProgram = LoadShaders(MB_VERTEX_SHADER_PATH, MB_FRAGMENT_SHADER_PATH);
	sphere = new OBJObject("../res/sphere.obj");
	obj = new SkymappingOBJ("../res/sphere.obj");
	fov = 45.0f;
	//cubeOBJ = new OBJObject("../cube.obj");
	culling = true;
	//singleRobot = buildRobot(glm::mat4(1.0f));
	curve = new Curve();
	controls = loadControls(glm::mat4(1.0f));
	obj->curvePts = curve->vertices;
	RefreshBallPos();	
	auto fbo = new SingleSampleFbo(Window::width, Window::height);
	ssFbo = new SingleSampleFbo(Window::width, Window::height);
	msFbo.push_back(fbo);
	glGenVertexArrays(1, &vaoQuad);
	glGenBuffers(1, &vboQuad);
	glBindBuffer(GL_ARRAY_BUFFER, vboQuad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glBindVertexArray(vaoQuad);
	glBindBuffer(GL_ARRAY_BUFFER, vboQuad);
	specifyScreenVertexAttributes(dofshaderProgram);
	specifyScreenVertexAttributes(mbshaderProgram);
	focal = 0.972656f;	
	Movement = Rotate;
}


void Window::RefreshBallPos()
{
	obj->setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(curve->topPoint)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f)) * glm::mat4(1.0f));
	obj->lastPoint = curve->topPoint;
	obj->index = curve->topIndex;
	obj->energy = 5.0f;
	shouldUpdate = false;
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(cube);
	delete(curve);
	delete(controls);
	delete(obj);	
	delete(ssFbo);

	glDeleteProgram(shaderProgram);
	glDeleteProgram(skyboxshaderProgram);
	glDeleteProgram(curveshaderProgram);
	glDeleteProgram(controlshaderProgram);
	glDeleteProgram(skymapShaderProgram);
	glDeleteProgram(dofshaderProgram);
	glDeleteProgram(mbshaderProgram);
	glDeleteVertexArrays(1, &vaoQuad);
	glDeleteBuffers(1, &vboQuad);

}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, glfwGetPrimaryMonitor(), NULL);
	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);
	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(fov, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
	//reinit fbo
	if (msFbo.size() > 0) {
		ssFbo->refresh(Window::width, Window::height);
		while (msFbo.size() > 0) {
			auto var = msFbo.front();
			delete(var);
			msFbo.pop_front();
		}
	}
	if (dofshaderProgram != 0) {
		glUniform1f(glGetUniformLocation(dofshaderProgram, "width"), Window::width);
		glUniform1f(glGetUniformLocation(dofshaderProgram, "height"), Window::height);
	}
	if (mbshaderProgram != 0) {
		glUniform1f(glGetUniformLocation(mbshaderProgram, "width"), Window::width);
		glUniform1f(glGetUniformLocation(mbshaderProgram, "height"), Window::height);
	}
}

void Window::idle_callback()
{
	double nowTime = glfwGetTime();
	//cout << "fps " << 1.0f / (nowTime - lastUpdatetime) << endl;
	if (shouldUpdate) {
		obj->update(nowTime - lastUpdatetime);
	}
	if (!shouldAllowManu) {
		cam_pos = obj->curvePts[obj->index] - glm::normalize(obj->facingDir) * 5.0f;
		P = glm::perspective(fov, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, obj->curvePts[obj->index] + (obj->facingDir) * 10.0f, cam_up);
	}
	dTime = nowTime - lastUpdatetime;
	lastUpdatetime = nowTime;
}
void Window::display_callback(GLFWwindow* window)
{
	glm::dvec2 currPos;
	glfwGetCursorPos(window, &currPos.x, &currPos.y);
	lastPoint = trackBallMapping(currPos.x, currPos.y);
	if (onlyfortest) return;
	glUniform1f(glGetUniformLocation(dofshaderProgram, "width"), Window::width);
	glUniform1f(glGetUniformLocation(dofshaderProgram, "height"), Window::height);
	glUniform1f(glGetUniformLocation(mbshaderProgram, "width"), Window::width);
	glUniform1f(glGetUniformLocation(mbshaderProgram, "height"), Window::height);
	
	//render a scene
	{
		glBindFramebuffer(GL_FRAMEBUFFER, ssFbo->fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		//normal draw
		curve->draw(curveshaderProgram, obj->friction);
		controls->draw(glm::mat4(1.0f));
		obj->draw(skymapShaderProgram);
		cube->draw(skyboxshaderProgram);
	}

	if (enableMB == 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindVertexArray(vaoQuad);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(dofshaderProgram);
		glUniform1i(glGetUniformLocation(dofshaderProgram, "openbabeijing"), enable8bj);
		glUniform1i(glGetUniformLocation(dofshaderProgram, "openDOF"), enableDOF);
		glUniform1f(glGetUniformLocation(dofshaderProgram, "focus"), Window::focal);
		//bind color map
		glUniform1i(glGetUniformLocation(dofshaderProgram, "texFramebuffer"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ssFbo->colormap);
		//bind depth map
		glUniform1i(glGetUniformLocation(dofshaderProgram, "tDepth"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, ssFbo->depthmap);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	else {
		if (msFbo.size() == MOTION_BLUR_SAMPLE_NUM) {
			auto lastone = msFbo.back();			
			delete(lastone);
			msFbo.pop_back();
		}
		//reder the scene to msFbo
		while (msFbo.size() < MOTION_BLUR_SAMPLE_NUM){			
			msFbo.push_front(new SingleSampleFbo(Window::width, Window::height));
			glBindFramebuffer(GL_FRAMEBUFFER, msFbo.front()->fbo);
			glBindVertexArray(vaoQuad);
			glDisable(GL_DEPTH_TEST);
			glUseProgram(dofshaderProgram);
			glUniform1i(glGetUniformLocation(dofshaderProgram, "openbabeijing"), enable8bj);
			glUniform1i(glGetUniformLocation(dofshaderProgram, "openDOF"), enableDOF);
			glUniform1f(glGetUniformLocation(dofshaderProgram, "focus"), Window::focal);
			//bind color map
			glUniform1i(glGetUniformLocation(dofshaderProgram, "texFramebuffer"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ssFbo->colormap);
			//bind depth map
			glUniform1i(glGetUniformLocation(dofshaderProgram, "tDepth"), 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, ssFbo->depthmap);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindVertexArray(vaoQuad);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(mbshaderProgram);
		//bind color map
		glUniform1i(glGetUniformLocation(mbshaderProgram, "numOfSample"), msFbo.size());
		int i = 0;
		typedef list<SingleSampleFbo*>::iterator FboIt;
		for (FboIt j = msFbo.begin(); j != msFbo.end(); ++j) {		
			char integer_string[32];
			sprintf(integer_string, "%d", i);
			char other_string[64] = "texFramebuffer"; 
			strcat(other_string, integer_string); 
			glUniform1i(glGetUniformLocation(mbshaderProgram, other_string), i);
			glActiveTexture(GL_TEXTURE0 + i);
			auto x = *j;
			glBindTexture(GL_TEXTURE_2D, x->colormap);
			i++;
		}
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}


	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{			
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key == GLFW_KEY_P) {			
			shouldUpdate = !shouldUpdate;
		}
		if (key == GLFW_KEY_SPACE) {
			RefreshBallPos();
		}
		if (key == GLFW_KEY_F) {
			obj->friction = !obj->friction;
		}
		if (key == GLFW_KEY_R) {
			cam_pos = glm::vec3(0.0f, 0.0f, 0.0f);
			xBiase = glm::vec3(1.0f, 0.0f, 0.0f);
			yBiase = glm::vec3(0.0f, 1.0f, 0.0f);
			zBiase = glm::vec3(0.0f, 0.0f, 1.0f);
			P = glm::perspective(fov, (float)width / (float)height, 0.1f, 1000.0f);
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
			shouldAllowManu = true;
		}
		if (key == GLFW_KEY_F1) {
			shouldAllowManu = false;
		}		
		if (key == GLFW_KEY_Q) {
			focal = 10000;
		}
		if (key == GLFW_KEY_E) {
			focal = 0.972656;
		}
		if (key == GLFW_KEY_1) {
			enableDOF = !enableDOF;
			cout << "DOF " << (enableDOF == 1 ? " Enabled" : "Disabled") << endl;
		}
		if (key == GLFW_KEY_2) {
			enableMB = !enableMB;
			cout << "Motion blur " << (enableMB == 1 ? " Enabled" : "Disabled") << endl;
			if (!enableMB) {
				while (msFbo.size() > 0) {
					auto var = msFbo.front();
					delete(var);
					msFbo.pop_front();
				}
			}
		}
	}
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Movement = Rotate;
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		enable8bj = 1;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		enable8bj = 0;
	}
}

glm::vec3 Window::trackBallMapping(double x, double y)    // The CPoint class is a specific Windows class. Either use separate x and y values for the mouse location, or use a Vector3 in which you ignore the z coordinate.
{
	//glm::vec3 v;    // Vector v is the synthesized 3D position of the mouse location on the trackball
	//float d;     // this is the depth of the mouse location: the delta between the plane through the center of the trackball and the z position of the mouse
	//v.x = (2.0*x - width) / width;   // this calculates the mouse X position in trackball coordinates, which range from -1 to +1
	//v.y = (2.0*y - height) / height;   // this does the equivalent to the above for the mouse Y position
	//v.z = 0.0;   // initially the mouse z position is set to zero, but this will change below
	//d = glm::length(v);    // this is the distance from the trackball's origin to the mouse location, without considering depth (=in the plane of the trackball's origin)
	//d = (d<1.0) ? d : 1.0;   // this limits d to values of 1.0 or less to avoid square roots of negative values in the following line
	//v.z = sqrtf(1.001 - d*d);  // this calculates the Z coordinate of the mouse position on the trackball, based on Pythagoras: v.z*v.z + d*d = 1*1
	//v = glm::normalize(v); // Still need to normalize, since we only capped d, not v.
	glm::vec3 v(x, y, 0);
	return v / 100.0f;  // return the mouse location on the surface of the trackball
}

void Window::cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
{
	//double xpos, ypos;
	//glfwGetCursorPos(window, &xpos, &ypos);
	glm::vec3 currPoint;
	if (!shouldAllowManu) {
		return;
	}
	if (Movement == Rotate) {
		currPoint = trackBallMapping(xpos, ypos);
		glm::vec3 direction = currPoint - lastPoint;
		float velocity = glm::length(direction);
		if (velocity > 0.0001) // If little movement - do nothing.
		{
			glm::vec3 rotAxis;
			rotAxis = glm::cross(lastPoint, currPoint);
			float rot_angle = velocity;
			float tempx, tempy;
			if (!mode) {
				tempx = xrot + (lastPoint.x - currPoint.x);
				tempy = yrot + (lastPoint.y - currPoint.y);
				if (tempy > glm::pi<float>() / 2 - 0.01f) {
					tempy = glm::pi<float>() / 2 - 0.01f;
				}
				if (tempy < -glm::pi<float>() / 2  + 0.01f) {
					tempy = -glm::pi<float>() / 2 + 0.01f;
				}
				dX = tempx - xrot;
				dY = tempy - yrot;
				xrot = tempx;
				yrot = tempy;
				cam_look_at = glm::rotate(glm::mat4(1.0f), xrot, glm::vec3(0.0f, 1.0f, 0.0f)) * 
					glm::rotate(glm::mat4(1.0f), yrot, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(glm::vec3(0.0f, 0.0f, -20.0f), 1.0f);
				P = glm::perspective(fov, (float)width / (float)height, 0.1f, 1000.0f);
				V = glm::lookAt(cam_pos, cam_look_at, cam_up);
			}
		}
		//cout << cam_pos.x << " " << cam_pos.y << " " << cam_pos.z << endl;
	}
	else if (Movement == Translate) {
		currPoint = glm::vec3(xpos, ypos, 0.0f);
		glm::vec3 direction = glm::vec3(currPoint.x - lastPoint.x, lastPoint.y - currPoint.y, currPoint.z - lastPoint.z);
		glm::vec3 test(xBiase.x * direction.x + yBiase.x * direction.y + zBiase.x * direction.z,
			xBiase.y * direction.x + yBiase.y * direction.y + zBiase.y * direction.z, 
			xBiase.z * direction.x + yBiase.z * direction.y + zBiase.z * direction.z);
		float velocity = glm::length(test);

		if (velocity > 0.001) {
			curve->points[selected] = glm::translate(glm::mat4(1.0f), test / 20.0f) * glm::vec4(curve->points[selected], 1.0f);
			if (selected % 3 == 1) {
				curve->points[(selected + 22) % 24] = curve->points[selected - 1] * 2.0f - curve->points[selected];
			}
			else if (selected % 3 == 2) {
				curve->points[(selected + 2) % 24] = curve->points[(selected + 1) % 24] * 2.0f - curve->points[selected];
			}
			else {
				curve->points[(selected + 23) % 24] = glm::translate(glm::mat4(1.0f), test / 20.0f) * glm::vec4(curve->points[(selected + 23) % 24], 1.0f);
				curve->points[(selected + 1) % 24] = glm::translate(glm::mat4(1.0f), test / 20.0f) * glm::vec4(curve->points[(selected + 1) % 24], 1.0f);
			}
			curve->refreshCurve();
			delete(controls);
			controls = loadControls(glm::mat4(1.0f));
			lastPoint = currPoint;
			obj->curvePts = curve->vertices;
			obj->setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(obj->curvePts[obj->index])) * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f)) * glm::mat4(1.0f));
			obj->lastPoint = obj->curvePts[obj->index];
		}
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset > 0) {
		focal += 1.0f / 256.0f;
	}
	else {
		focal -= 1.0f / 256.0f;

	}
	P = glm::perspective(fov, (float)width / (float)height, 0.1f, 1000.0f);
	V = glm::lookAt(cam_pos, cam_look_at, cam_up);
}

Transform * Window::loadControls(glm::mat4)
{
	Transform* result = new Transform();
	glm::vec3 points[24];
	for (int i = 0; i < 24; i++) {
		points[i] = curve->points[i];
		if (i % 3 == 0) {
			Transform * redControl = new Transform();
			redControl->M = glm::translate(glm::mat4(1.0f), points[i]) * 
				glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)) * glm::mat4(1.0f);
			result->addChild(redControl);
			Geometry * control = new Geometry(sphere, controlshaderProgram, (i + 1) * 10, "inter");
			redControl->addChild(control);
		}
		else {
			Transform * greenControl = new Transform();
			greenControl->M = glm::translate(glm::mat4(1.0f), points[i]) *
				glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)) * glm::mat4(1.0f);
			result->addChild(greenControl);
			Geometry * control = new Geometry(sphere, controlshaderProgram, (i + 1) * 10, "appro");
			greenControl->addChild(control);
		}
	}
	return result;
}

//Transform * Window::buildRobot(glm::mat4 position)
//{
//	Transform * newRobot = new Transform("robot");
//	newRobot->M = newRobot->M * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
//	Transform * headTransform = new Transform("head");
//	headTransform->M = glm::rotate(glm::mat4(1.0f), -glm::pi<float>()*0.5f, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::mat4(1.0f);
//	headTransform->addChild(new Geometry(head, shaderProgram, "head"));
//	newRobot->addChild(headTransform);
//	Transform * bodyTransform = new Transform("body");
//	bodyTransform->M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -35.0f, 0.0f)) *
//		glm::rotate(glm::mat4(1.0f), -glm::pi<float>()*0.5f, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::mat4(1.0f);
//	bodyTransform->addChild(new Geometry(body, shaderProgram, "body"));
//	newRobot->addChild(bodyTransform);
//	limbTransformL = new Transform("limbL");
//	robotArm = limbTransformL;
//	limbTransformL->addChild(new Geometry(limb, shaderProgram, "limb", true));
//	limbTransformL->M = glm::translate(glm::mat4(1.0f), glm::vec3(-16.0f, -30.0f, 0.0f)) * 
//		glm::rotate(glm::mat4(1.0f), -glm::pi<float>()*0.5f, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::mat4(1.0f);
//	newRobot->addChild(limbTransformL);
//	limbTransformR = new Transform("limbR");
//	limbTransformR->M = glm::translate(glm::mat4(1.0f), glm::vec3(38.0f, -30.0f, 0.0f)) *
//		glm::rotate(glm::mat4(1.0f), -glm::pi<float>()*0.5f, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::mat4(1.0f);
//	limbTransformR->addChild(new Geometry(limb, shaderProgram, "limb", false));
//	newRobot->addChild(limbTransformR);
//	limbTransformLL = new Transform("limbLL");
//	limbTransformLL->M = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, -70.0f, 0.0f)) *
//		glm::rotate(glm::mat4(1.0f), -glm::pi<float>()*0.5f, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::mat4(1.0f);
//	limbTransformLL->addChild(new Geometry(limb, shaderProgram, "limb", false));
//	newRobot->addChild(limbTransformLL);
//	limbTransformRR = new Transform("limbRR");
//	limbTransformRR->M = glm::translate(glm::mat4(1.0f), glm::vec3(25.0f, -70.0f, 0.0f)) *
//		glm::rotate(glm::mat4(1.0f), -glm::pi<float>()*0.5f, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::mat4(1.0f);
//	limbTransformRR->addChild(new Geometry(limb, shaderProgram, "limb", true));
//	newRobot->addChild(limbTransformRR);
//	Transform * eyeTransformL = new Transform("eyeL");
//	eyeTransformL->M = glm::translate(glm::mat4(1.0f), glm::vec3(-8.0f, 7.0f, 12.0f)) * glm::mat4(1.0f);
//	eyeTransformL->addChild(new Geometry(eye, shaderProgram, "eye"));
//	newRobot->addChild(eyeTransformL);
//	Transform * eyeTransformR = new Transform("eyeR");
//	eyeTransformR->M = glm::translate(glm::mat4(1.0f), glm::vec3(8.0f, 7.0f, 12.0f)) * glm::mat4(1.0f);
//	eyeTransformR->addChild(new Geometry(eye, shaderProgram, "eye"));
//	newRobot->addChild(eyeTransformR);
//	Transform * antL = new Transform("antL");
//	antL->M = glm::translate(glm::mat4(1.0f), glm::vec3(-8.0f, 7.0f, 0.0f)) *
//		glm::rotate(glm::mat4(1.0f), glm::pi<float>() * -0.5f, glm::vec3(1.0f, 0.0f, 0.0f))*
//		glm::mat4(1.0f);
//	antL->addChild(new Geometry(ant, shaderProgram, "ant"));
//	newRobot->addChild(antL);
//	Transform * antR = new Transform("antR");
//	antR->M = glm::translate(glm::mat4(1.0f), glm::vec3(8.0f, 7.0f, 0.0f)) *
//		glm::rotate(glm::mat4(1.0f), glm::pi<float>() * -0.5f, glm::vec3(1.0f, 0.0f, 0.0f))*
//		glm::rotate(glm::mat4(1.0f), glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f))*
//		glm::mat4(1.0f);
//	antR->addChild(new Geometry(ant, shaderProgram, "ant"));
//	newRobot->addChild(antR);
//	Transform * sph = new Transform("sphere");
//	sph->M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -35.0f, 0.0f)) *
//		glm::scale(glm::mat4(1.0f), glm::vec3(40.0f, 60.0f, 50.0f)) * glm::mat4(1.0f);
//	sph->addChild(new Geometry(sphere, shaderProgram, "sph"));
//	newRobot->addChild(sph);
//	//Transform * cubeTrans = new Transform("cube");
//	//cubeTrans->M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -35.0f, 0.0f)) *
//	//	glm::scale(glm::mat4(1.0f), glm::vec3(80.0f, 80.0f, 80.0f)) * glm::mat4(1.0f);
//	//cubeTrans->addChild(new Geometry(cubeOBJ, shaderProgram, "cube"));
//	//newRobot->addChild(cubeTrans);
//	return newRobot;
//}

