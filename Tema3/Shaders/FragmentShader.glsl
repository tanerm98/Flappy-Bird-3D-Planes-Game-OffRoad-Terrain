#version 330
 
uniform sampler2D texture_2;
in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	out_color = texture2D(texture_2, texcoord); 
}