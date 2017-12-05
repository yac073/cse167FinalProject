#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;
out float z;
out float test;

uniform mat4 projection;
uniform mat4 view;
uniform float focal;

void main()
{
    TexCoords = aPos;
	test = 0;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
	if (focal >= 500) { z = 0.0; } else 
	if (focal >= 250) { z = 0.5; } else
	if (focal >= 200) { z = 1; } else 
	if (focal >= 150) { z = 1.5; } else 
	if (focal >= 150) { z = 2; } else 
	if (focal >= 50) { z = 2.5; } else 
	{
		z = length(view * vec4(aPos, 1.0)) - focal;		
	}
	//if (abs(z) > 0){test = abs(z);}
} 
