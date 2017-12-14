#version 330 core
in vec3 fragposition;
in vec3 TexCoords;
in vec3 fragNormal;
in float z;
out vec4 color;
uniform int isInter;
uniform int isAlpha;
uniform int id;
uniform mat4 model;
uniform vec3 v;

uniform struct Light
{
	vec3 direction;
	vec3 color;
}light;

uniform samplerCube tex;
uniform samplerCube tex2;

void main(){
  vec4 final_color;
  if (isAlpha == 1){
    color = vec4(id / 255.0f, 0.0f, 0.0f, 1.0f);
  } else {  
    if (isInter == 0){
      final_color = texture(tex, TexCoords);
    }
    else {
      final_color = texture(tex2, TexCoords);
    }
		mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 normal = normalize(normalMatrix * fragNormal);

	vec3 fragPosition = vec3(model * vec4(fragposition,1));

	vec3 viewDir = normalize(v - fragPosition);

	vec3 lightDir = normalize(-light.direction);
	vec3 diffuse = vec3(final_color) * max(dot(lightDir,normal),0.0);

	vec3 ambient = 0.1 * vec3(final_color);

	float intensity = max(dot(lightDir,normal),0.0);

	if (intensity > 1.0)
		intensity = 1.0;
	else if (intensity > 0.9)
		intensity = 0.9;
	else if (intensity > 0.7)
		intensity = 0.7;
	else if (intensity > 0.5)
		intensity = 0.5;
	else if (intensity > 0.3)
		intensity = 0.3;
	else if (intensity > 0.1)
		intensity = 0.1;

	diffuse = vec3(final_color) * intensity;


	float edge = max(dot(viewDir,normal),0.0);
	//if (edge < 0.01){
	//	color = vec4(0.0, 0.0, 0.0, 1.0f);
	//} else{
		color = vec4(ambient + diffuse,1.0f);
	//}
  }
}
