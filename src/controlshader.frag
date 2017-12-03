#version 330 core
in vec3 Normal;
out vec4 color;
uniform int isInter;
uniform int isAlpha;
uniform int id;
void main(){
  if (isAlpha == 1){
    color = vec4(id / 255.0f, 0.0f, 0.0f, 1.0f);
  } else {  
    if (isInter == 1){
      color = vec4(1.0f,0.0f,0.0f,1.0f);
    }
    else {
      color = vec4 (0.0f,1.0f,0.0f,1.0f);
    }
  }
}
