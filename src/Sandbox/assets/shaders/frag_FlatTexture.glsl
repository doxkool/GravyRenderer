#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// texture sampler
uniform sampler2D u_texture;

void main()
{
	FragColor = texture(u_texture, TexCoords);
}