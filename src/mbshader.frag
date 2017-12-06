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
uniform sampler2D texFramebuffer9;
uniform sampler2D texFramebuffer10;
uniform sampler2D texFramebuffer11;
uniform sampler2D texFramebuffer12;
uniform sampler2D texFramebuffer13;
uniform sampler2D texFramebuffer14;
uniform sampler2D texFramebuffer15;
uniform sampler2D texFramebuffer16;
uniform sampler2D texFramebuffer17;
uniform sampler2D texFramebuffer18;
uniform sampler2D texFramebuffer19;
uniform sampler2D texFramebuffer20;
uniform sampler2D texFramebuffer21;
uniform sampler2D texFramebuffer22;
uniform sampler2D texFramebuffer23;
uniform sampler2D texFramebuffer24;
void main(){
	float s = float(numOfSample);
	outColor = texture(texFramebuffer0, Texcoord) * (0.75f / 15) + 
	texture(texFramebuffer1, Texcoord) * (0.75f / 15) +
	texture(texFramebuffer2, Texcoord) * (0.75f / 15) +
	texture(texFramebuffer3, Texcoord) * (0.75f / 15) +
	texture(texFramebuffer4, Texcoord) * (0.75f / 15) +
	texture(texFramebuffer5, Texcoord) * (0.75f / 15) +
	texture(texFramebuffer6, Texcoord) * (0.75f / 15) +
	texture(texFramebuffer7, Texcoord) * (0.75f / 15) +
	texture(texFramebuffer8, Texcoord) * (0.75f / 15) +
	texture(texFramebuffer9, Texcoord) * (0.75f / 15) +
	texture(texFramebuffer10, Texcoord) * (0.75f / 15) +
	texture(texFramebuffer11, Texcoord) * (0.75f / 15) +
	texture(texFramebuffer12, Texcoord) * (0.75f / 15) +
	texture(texFramebuffer13, Texcoord) * (0.75f / 15) +
	texture(texFramebuffer14, Texcoord) * (0.75f / 15) +
	texture(texFramebuffer15, Texcoord) * (0.25f / 10) +
	texture(texFramebuffer16, Texcoord) * (0.25f / 10) +
	texture(texFramebuffer17, Texcoord) * (0.25f / 10) +
	texture(texFramebuffer18, Texcoord) * (0.25f / 10) +
	texture(texFramebuffer19, Texcoord) * (0.25f / 10) +
	texture(texFramebuffer20, Texcoord) * (0.25f / 10) +
	texture(texFramebuffer21, Texcoord) * (0.25f / 10) +
	texture(texFramebuffer22, Texcoord) * (0.25f / 10) +
	texture(texFramebuffer23, Texcoord) * (0.25f / 10) +
	texture(texFramebuffer24, Texcoord) * (0.25f / 10)
	;	
	outColor.a = 1.0;	
}
