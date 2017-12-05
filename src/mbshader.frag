#version 330 core
in vec2 Texcoord;
out vec4 outColor;
uniform int numOfSample;
uniform sampler2D texFramebuffer0;
uniform sampler2D texFramebuffer1;
uniform sampler2D texFramebuffer2;
uniform sampler2D texFramebuffer3;
uniform sampler2D texFramebuffer4;
uniform sampler2D texFramebuffer5;
uniform sampler2D texFramebuffer6;
uniform sampler2D texFramebuffer7;
uniform sampler2D texFramebuffer8;
void main(){
	float s = float(numOfSample);
	outColor = texture(texFramebuffer0, Texcoord) / s + 
	texture(texFramebuffer1, Texcoord) / s +
	texture(texFramebuffer2, Texcoord) / s +
	texture(texFramebuffer3, Texcoord) / s +
	texture(texFramebuffer4, Texcoord) / s +
	texture(texFramebuffer5, Texcoord) / s +
	texture(texFramebuffer6, Texcoord) / s +
	texture(texFramebuffer7, Texcoord) / s +
	texture(texFramebuffer8, Texcoord) / s;	
	outColor.a = 1.0;	
}
