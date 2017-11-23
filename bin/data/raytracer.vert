#version 150

uniform mat4 modelViewProjectionMatrix;
uniform sampler2DRect uBodyIndexTex;
uniform float lookalike;
uniform int uWidth;

in vec4 position;
in vec2 texcoord;

out Vertex
{
	vec2 vTexCoord;
	float vKeep;
} vertex;

void main(){
	gl_Position = position;
	vec2 idxTexCoord = vec2(gl_VertexID % uWidth, gl_VertexID / uWidth);
	vec4 idxColor = texture(uBodyIndexTex, idxTexCoord);
	if (idxColor.r == 1.0 || gl_VertexID % int(10 / int(clamp(10 * (1.0 - lookalike), 1, 10))) != 0) 
	{
		vertex.vKeep = 0.0;
	}
	else 
	{
		vertex.vKeep = 1.0;
	}
	vertex.vTexCoord = texcoord;
}
