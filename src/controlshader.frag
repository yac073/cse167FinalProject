#version 330 core
in vec3 Normal;
in float z;
out vec4 color;
uniform int isInter;
uniform int isAlpha;
uniform int id;
void main(){
  if (isAlpha == 1){
    color = vec4(id / 255.0f, 0.0f, 0.0f, 1.0f);
  } else {  
  	float c_z = 1.0f / abs(z);
    if (isInter == 1){
	  //color = vec4(c_z);
      color = vec4(1.0f,0.0f,0.0f,c_z);
    }
    else {
	  //color = vec4(c_z);
      color = vec4 (0.0f,1.0f,0.0f,c_z);
    }
  }
}
