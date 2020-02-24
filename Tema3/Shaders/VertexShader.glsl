#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_coord;
layout(location = 3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform sampler2D texture_1;

out vec2 texcoord;

void main()
{
	float height = texture2D(texture_1, v_coord).r;;
	texcoord = v_coord;
	gl_Position = Projection * View * Model * vec4(vec3(v_position.x, height * 7, v_position.z), 1.0);
}