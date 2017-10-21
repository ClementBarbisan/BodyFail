#version 150

uniform sampler2DRect tex0;
in vec2 vTexCoord;
out vec4 vFragColor;

void main()
{
	vFragColor = texture(tex0, vec2(1.0f - vTexCoord.x, vTexCoord.y));
}