#version 330 core
in vec3 Normal;
in vec3 TexCoords;
in float z;
out vec4 color;
uniform int isInter;
uniform int isAlpha;
uniform int id;

uniform samplerCube tex;
uniform samplerCube tex2;

void main(){
  if (isAlpha == 1){
    color = vec4(id / 255.0f, 0.0f, 0.0f, 1.0f);
  } else {  
    if (isInter == 0){
      color = texture(tex, TexCoords);
    }
    else {
      color = texture(tex2, TexCoords);
    }
  }
}
