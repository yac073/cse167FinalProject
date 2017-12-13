#version 450 core

uniform struct Light
{
	vec3 direction;
	vec3 color;
}light;

uniform mat4 model;

in vec3 TexCoords;
in vec3 fragNormal;
in vec3 fragposition;

out vec4 color;

uniform samplerCube tex;

void main()
{
	vec4 final_color = texture(tex, TexCoords);

	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 normal = normalize(normalMatrix * fragNormal);

	vec3 fragPosition = vec3(model * vec4(fragposition,1));

	vec3 viewDir = normalize(vec3(0.0, 0.0, 20.0) - fragPosition);

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


	color = vec4(diffuse + ambient,1.0f);
	//color = vec4(1.0f);
}
