#version 330 core
out vec4 FragColor;

in vec3 TexCoords;
in float z;
in float test;

uniform samplerCube skybox;

void main()
{    
    vec4 color = texture(skybox, TexCoords);
    FragColor = vec4(color.x,color.y,color.z, 1.0f / abs(z));
}
