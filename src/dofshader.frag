#version 330 core
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;
uniform sampler2D tDepth;
uniform float width;
uniform float height;
uniform int openDOF;
uniform float focus;
uniform int openbabeijing;
void enablebabeijing(){
    if (abs(Texcoord.x - 0.5f) < 1.0f / width && abs(Texcoord.y - 0.5f) > 30.0f / height) { outColor = vec4(0.0f);}
	if (abs(Texcoord.y - 0.5f) < 1.0f / height && abs(Texcoord.x - 0.5f) > 30.0f / width) { outColor = vec4(0.0f);}
	if (abs(Texcoord.x - 0.5f) < 2.0f / width && abs(Texcoord.y - 0.5f) < 2.0f / height) { outColor = vec4(0.0f);}		
	float yDis = abs(Texcoord.y * height - height / 2);
	float xDis = abs(Texcoord.x * width - width / 2);
	if (xDis * xDis + yDis * yDis > height * height / 4  ) { outColor = vec4(0.0f);}
	int y2 = int(Texcoord.y * height);
	int hei = int(height / 10);
	if (y2 % (hei) == 1){
		if (abs(Texcoord.x - 0.5f) < abs(y2 * 2 / hei - 10) * 2 / height) { 			
			outColor = vec4(0.0f);
		}
	}
	if (xDis * xDis + yDis * yDis > height * height / (36 * 36) && xDis * xDis + yDis * yDis < height * height / (33 * 33)  ) { outColor = vec4(1.0f, 0.0f, 0.0f, 0.0f);}
}
vec4 dof(vec4 col, vec2 aspectcorrect, vec2 dofblur){    
    vec2 dofblur9 = dofblur * 0.9;
    vec2 dofblur7 = dofblur * 0.7;
    vec2 dofblur4 = dofblur * 0.4;
	col += texture( texFramebuffer, Texcoord + ( vec2( 0.0f, 0.4f ) * aspectcorrect ) * dofblur );
	col += texture( texFramebuffer, Texcoord + ( vec2( 0.15, 0.37 ) * aspectcorrect ) * dofblur );
	col += texture( texFramebuffer, Texcoord + ( vec2( 0.29, 0.29 ) * aspectcorrect ) * dofblur );
    col += texture( texFramebuffer, Texcoord + ( vec2( -0.37, 0.15 ) * aspectcorrect ) * dofblur );
    col += texture( texFramebuffer, Texcoord + ( vec2( 0.40, 0.0 ) * aspectcorrect ) * dofblur );
    col += texture( texFramebuffer, Texcoord + ( vec2( 0.37, -0.15 ) * aspectcorrect ) * dofblur );
    col += texture( texFramebuffer, Texcoord + ( vec2( 0.29, -0.29 ) * aspectcorrect ) * dofblur );
    col += texture( texFramebuffer, Texcoord + ( vec2( -0.15, -0.37 ) * aspectcorrect ) * dofblur );
    col += texture( texFramebuffer, Texcoord + ( vec2( 0.0, -0.4 ) * aspectcorrect ) * dofblur );
    col += texture( texFramebuffer, Texcoord + ( vec2( -0.15, 0.37 ) * aspectcorrect ) * dofblur );
    col += texture( texFramebuffer, Texcoord + ( vec2( -0.29, 0.29 ) * aspectcorrect ) * dofblur );
    col += texture( texFramebuffer, Texcoord + ( vec2( 0.37, 0.15 ) * aspectcorrect ) * dofblur );
    col += texture( texFramebuffer, Texcoord + ( vec2( -0.4, 0.0 ) * aspectcorrect ) * dofblur );
    col += texture( texFramebuffer, Texcoord + ( vec2( -0.37, -0.15 ) * aspectcorrect ) * dofblur );
    col += texture( texFramebuffer, Texcoord + ( vec2( -0.29, -0.29 ) * aspectcorrect ) * dofblur );
    col += texture( texFramebuffer, Texcoord + ( vec2( 0.15, -0.37 ) * aspectcorrect ) * dofblur );
    col += texture( texFramebuffer, Texcoord + ( vec2( 0.15, 0.37 ) * aspectcorrect ) * dofblur9 );
    col += texture( texFramebuffer, Texcoord + ( vec2( -0.37, 0.15 ) * aspectcorrect ) * dofblur9 );
    col += texture( texFramebuffer, Texcoord + ( vec2( 0.37, -0.15 ) * aspectcorrect ) * dofblur9 );
    col += texture( texFramebuffer, Texcoord + ( vec2( -0.15, -0.37 ) * aspectcorrect ) * dofblur9 );
    col += texture( texFramebuffer, Texcoord + ( vec2( -0.15, 0.37 ) * aspectcorrect ) * dofblur9 );
	col += texture( texFramebuffer, Texcoord + ( vec2( 0.37, 0.15 ) * aspectcorrect ) * dofblur9 );
	col += texture( texFramebuffer, Texcoord + ( vec2( -0.37, -0.15 ) * aspectcorrect ) * dofblur9 );
	col += texture( texFramebuffer, Texcoord + ( vec2( 0.15, -0.37 ) * aspectcorrect ) * dofblur9 );
	col += texture( texFramebuffer, Texcoord + ( vec2( 0.29, 0.29 ) * aspectcorrect ) * dofblur7 );
	col += texture( texFramebuffer, Texcoord + ( vec2( 0.40, 0.0 ) * aspectcorrect ) * dofblur7 );
	col += texture( texFramebuffer, Texcoord + ( vec2( 0.29, -0.29 ) * aspectcorrect ) * dofblur7 );
	col += texture( texFramebuffer, Texcoord + ( vec2( 0.0, -0.4 ) * aspectcorrect ) * dofblur7 );
	col += texture( texFramebuffer, Texcoord + ( vec2( -0.29, 0.29 ) * aspectcorrect ) * dofblur7 );
	col += texture( texFramebuffer, Texcoord + ( vec2( -0.4, 0.0 ) * aspectcorrect ) * dofblur7 );
	col += texture( texFramebuffer, Texcoord + ( vec2( -0.29, -0.29 ) * aspectcorrect ) * dofblur7 );
	col += texture( texFramebuffer, Texcoord + ( vec2( 0.0, 0.4 ) * aspectcorrect ) * dofblur7 );
	col += texture( texFramebuffer, Texcoord + ( vec2( 0.29, 0.29 ) * aspectcorrect ) * dofblur4 );
	col += texture( texFramebuffer, Texcoord + ( vec2( 0.4, 0.0 ) * aspectcorrect ) * dofblur4 );
	col += texture( texFramebuffer, Texcoord + ( vec2( 0.29, -0.29 ) * aspectcorrect ) * dofblur4 );
	col += texture( texFramebuffer, Texcoord + ( vec2( 0.0, -0.4 ) * aspectcorrect ) * dofblur4 );
	col += texture( texFramebuffer, Texcoord + ( vec2( -0.29, 0.29 ) * aspectcorrect ) * dofblur4 );
	col += texture( texFramebuffer, Texcoord + ( vec2( -0.4, 0.0 ) * aspectcorrect ) * dofblur4 );
	col += texture( texFramebuffer, Texcoord + ( vec2( -0.29, -0.29 ) * aspectcorrect ) * dofblur4 );
	col += texture( texFramebuffer, Texcoord + ( vec2( 0.0, 0.4 ) * aspectcorrect ) * dofblur4 );
    return col / 41.0;
}
void main(){
	vec4 testColor = texture(texFramebuffer, Texcoord);	
	
    vec2 aspectcorrect = vec2( 1.0, 16.0f / 9.0f );
	vec4 depth1 = texture2D(tDepth, Texcoord );
    
    float factor = depth1.x - focus;
    
    vec2 dofblur = vec2 ( clamp( factor * 1/5.6f, -0.05f, 0.05f ) );
    
    vec4 col = vec4( 0.0f );
    col += texture(texFramebuffer, Texcoord);
	if (openDOF == 1){
		col = dof(col, aspectcorrect, dofblur);
	}	
	outColor = col;
	if (openbabeijing == 1){
		enablebabeijing();
	}	
	outColor.a = 1.0;	
}
