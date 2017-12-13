#version 330 core
in vec3 Normal;
out vec4 color;
uniform int isInter;
void main(){
  if (isInter == 1){
	  color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
  } else {
      color = vec4(0.0f,1.0f,0.0f,1.0f);
  }
}
