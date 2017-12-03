#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

// You can output many things. The first vec4 type output determines the color of the fragment

struct Light
{
	vec3 Color;
	vec3 Direction;
	vec3 Position;
	vec3 ConeCenter;
	float Cutoff;
	float Exponent;
	int Type;
};
struct Material
{
	vec3 Diffuse;
	vec3 Ambient;
	vec3 Specular;
	vec3 Color;
	float Shininess;
	int Type;
};
in float sampleExtraOutput;
in vec3 Normal;
in vec3 FragPosition;
uniform vec3 viewPosition;
uniform Light light;
uniform Material material;
uniform bool useNormal;
uniform float transparancy;
out vec4 color;

void main()
{
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
	if (useNormal){
		color = vec4(Normal, transparancy);
	} else {
		if (light.Type == 0){
			vec3 ambient = material.Ambient * light.Color * material.Color;
			vec3 lightDirection = normalize(-light.Direction);  
			float diff = max(dot(Normal, lightDirection), 0.0);
			vec3 diffuse = diff * material.Diffuse * light.Color;
			vec3 viewDir = normalize(viewPosition - FragPosition);
			vec3 reflectDir = reflect(-lightDirection, Normal);  
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
			vec3 specular = material.Specular * spec * light.Color;  
			vec3 result = ambient + diffuse + specular;
			color = vec4(result, sampleExtraOutput);
		} else if (light.Type == 1){
			float distance = length(light.Position - FragPosition);
			float attenuation = 1.0f / (1.0f + 0.01f * distance); 
			vec3 ambient = material.Ambient * light.Color * material.Color;
			vec3 lightDirection = normalize(light.Position - FragPosition);  
			float diff = max(dot(Normal, lightDirection), 0.0);
			vec3 diffuse = diff * material.Diffuse * light.Color;
			vec3 viewDir = normalize(viewPosition - FragPosition);
			vec3 reflectDir = reflect(-lightDirection, Normal);  
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
			vec3 specular = material.Specular * spec * light.Color;  
			vec3 result = ambient + diffuse + specular;
			color = vec4(result * attenuation , sampleExtraOutput);
		} else if (light.Type == 2){
			float distance = length(light.Position - FragPosition);
			float attenuation = 1.0f / (1.0f + 0.001f * distance * distance); 
			vec3 lightDirection = normalize(light.Position - FragPosition);  
			float angle = dot(lightDirection, -normalize(light.ConeCenter));
			if (angle > light.Cutoff){
				vec3 ambient = material.Ambient * light.Color * material.Color;
				vec3 lightDirection = normalize(- light.ConeCenter);  
				float diff = max(dot(Normal, lightDirection), 0.0);
				vec3 diffuse = diff * material.Diffuse * light.Color;
				vec3 viewDir = normalize(viewPosition - FragPosition);
				vec3 reflectDir = reflect(-lightDirection, Normal);  
				float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
				vec3 specular = material.Specular * spec * light.Color;  
				vec3 result = (ambient + diffuse + specular) * attenuation * pow(angle, light.Exponent);
				color = vec4(result, sampleExtraOutput);
			} else {
				vec3 ambient = material.Ambient * light.Color * material.Color;
				color = vec4(ambient, sampleExtraOutput);
			}
		}
	}
}
