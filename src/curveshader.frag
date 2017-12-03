#version 330 core
in vec3 Normal;
out vec4 color;
uniform int isCurve;
uniform int hasFric;
void main(){
  if (isCurve == 1){
    if (hasFric == 1){
	  color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	} else {
      color = vec4(0.0f,0.0f,0.0f,1.0f);
	}
  }
  else {
    color = vec4 (1.0f,1.0f,0.0f,1.0f);
  }
}
