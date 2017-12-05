#include "fbo.h"

SingleSampleFbo::SingleSampleFbo(int width, int height)
{
	initFunc(width, height);
}

void SingleSampleFbo::initFunc(int width, int height)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Create two textures to hold color buffers
	glGenTextures(1, &depthmap);
	glGenTextures(1, &colormap);

	// Set up the first framebuffer's depth buffer
	glBindTexture(GL_TEXTURE_2D, depthmap);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthmap, 0);

	// Set up the second framebuffer's color buffer
	glBindTexture(GL_TEXTURE_2D, colormap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colormap, 0);



	// check error
	GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (err == GL_FRAMEBUFFER_COMPLETE) {
		//cout << "fbo ready!" << endl;
	}
}

SingleSampleFbo::~SingleSampleFbo()
{
	deleteFunc();
}

void SingleSampleFbo::deleteFunc()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &depthmap);
	glDeleteTextures(1, &colormap);
}

void SingleSampleFbo::refresh(int width, int height)
{
	deleteFunc();
	initFunc(width, height);
}
