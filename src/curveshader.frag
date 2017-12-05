#version 330 core
in vec3 Normal;
in float z;
out vec4 color;
uniform int isCurve;
uniform int hasFric;
void main(){
float c_z = 1.0f / abs(z);
  if (isCurve == 1){
    if (hasFric == 1){
	  color = vec4(0.0f, 0.0f, 1.0f, c_z);
	} else {
      color = vec4(0.0f,0.0f,0.0f,c_z);
	}
  }
  else {
    color = vec4 (1.0f,1.0f,0.0f,c_z);
  }
}
