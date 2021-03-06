#version 150

// OF default uniforms and attributes
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform float lookalike;

in vec4 position;
in vec2 texcoord;
    	
// App uniforms and attributes
uniform sampler2DRect uBodyIndexTex;
uniform int uWidth;

out float vKeep;
out vec2 vTexCoord;


out Vertex
{
	vec2 vTexCoord;
	float vKeep;
	vec4 idxColor;
	int id;
} vertex;

void main()
{ 
	gl_Position.xy = position.xy;
	gl_Position.z = position.z;
	gl_Position.w = 0.0;
	vertex.vTexCoord = texcoord;

	vec2 idxTexCoord = vec2(gl_VertexID % uWidth, gl_VertexID / uWidth);
	vec4 idxColor = texture(uBodyIndexTex, idxTexCoord);
	vertex.idxColor = idxColor;
	vertex.id = gl_VertexID;
	if (idxColor.r > 0.75f || gl_VertexID % int(10 / int(clamp(10 * (1.0 - lookalike), 1, 10))) != 0) 
	{
		vertex.vKeep = 0.0;
	}
	else {
		vertex.vKeep = 1.0;
	}
}
