#version 450 core

uniform struct Light
{
	vec3 direction;
	vec3 color;
}light;

uniform mat4 model;
uniform float random;

in vec2 TexCoords;
in vec3 fragNormal;
in vec3 fragposition;

out vec4 color;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
uniform sampler2D tex_blend;
uniform sampler2D tex_blend2;
uniform sampler2D tex_blend3;
uniform sampler2D tex_blend4;
uniform sampler2D tex_blend5;

void main()
{
	//vec4 blendmapcolor;
	//if(random == 0)
	//	blendmapcolor = texture(tex_blend, TexCoords);
	//else if(random == 1)
//		blendmapcolor = texture(tex_blend2, TexCoords);
	//else if(random == 2)
//		blendmapcolor = texture(tex_blend3, TexCoords);
	//else if(random == 3)
//		blendmapcolor = texture(tex_blend4, TexCoords);
	//else
		//blendmapcolor = texture(tex_blend5, TexCoords);

	//float backAmount = 1.0 - (blendmapcolor.r + blendmapcolor.g + blendmapcolor.b);

	vec2 tiledCoords = TexCoords * 40.0;
	//vec4 color1 = texture(tex1, tiledCoords) * backAmount;
	//vec4 color2 = texture(tex2, tiledCoords) * blendmapcolor.r;
	//vec4 color3 = texture(tex3, tiledCoords) * blendmapcolor.g;
	//vec4 color4 = texture(tex4, tiledCoords) * blendmapcolor.b;

	vec4 final_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	float height = (fragposition.y + 60.0f) / 80.0f;
	if (height <= 0.3){
		if (height < 0.25){
			final_color += texture(tex1, tiledCoords);
		} else {
			final_color += texture(tex1, tiledCoords) * (0.3f - height) / 0.05f;
		}
	}
	if (height > 0.25 && height <= 0.55){
		if (height > 0.25 && height < 0.3){
			final_color += texture(tex3, tiledCoords) * (height - 0.25) / 0.05f;
		} else if (height < 0.5){
			final_color += texture(tex3, tiledCoords);
		} else {
			final_color += texture(tex3, tiledCoords) * (0.55f - height) / 0.05f;
		}
	}
	if (height > 0.5 && height <= 0.8){
		if (height > 0.5 && height < 0.55){
			final_color += texture(tex2, tiledCoords) * (height - 0.5) / 0.05f;
		} else if (height < 0.75){
			final_color += texture(tex2, tiledCoords);
		} else {
			final_color += texture(tex2, tiledCoords) * (0.8f - height) / 0.05f;
		}
	}
	if (height > 0.75){
		if (height > 0.75 && height < 0.8){
			final_color += texture(tex4, tiledCoords) * (height - 0.75) / 0.05f;
		} else {
			final_color += texture(tex4, tiledCoords);
		}
	}

	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 normal = normalize(normalMatrix * fragNormal);

	vec3 fragPosition = vec3(model * vec4(fragposition,1));

	vec3 viewDir = normalize(vec3(0.0, 0.0, 20.0) - fragPosition);

	vec3 lightDir = normalize(-light.direction);

	float intensity = max(dot(lightDir,normal),0.0);
	vec3 diffuse = vec3(final_color) * intensity;

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

	//vec3 diffuse = vec3(final_color) * intensity;

	vec3 ambient = 0.15 * vec3(final_color);
	color = vec4(ambient + diffuse,1.0f);
	//color = vec4(vec3((fragposition.y + 5.0f) / 40.0f),1.0f);
}
