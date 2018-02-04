#version 150

uniform sampler2DRect tex0;
uniform float lookalike;

in vec2 texCoordVarying;
out vec4 outputColor;

void main()
{

    vec4 color;
	vec4 colorTex = texture(tex0, texCoordVarying);
	if (colorTex != texture(tex0, vec2(texCoordVarying.x, texCoordVarying.y + 0.01)) || colorTex != texture(tex0, vec2(texCoordVarying.x, texCoordVarying.y - 0.01)) 
		|| colorTex != texture(tex0, vec2(texCoordVarying.x + 0.01, texCoordVarying.y)) || colorTex != texture(tex0, vec2(texCoordVarying.x - 0.01, texCoordVarying.y)))
		color = vec4(1.0 * lookalike - 0.1);
	else
		color = vec4(0.0);
    outputColor = color;
}