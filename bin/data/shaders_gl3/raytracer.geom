#version 150 core
layout(points) in;
layout(line_strip, max_vertices=2) out;

uniform float time;
uniform mat4 modelViewProjectionMatrix;

in Vertex
{
	vec2 vTexCoord;
	float vKeep;
} vertex[];

out float vKeep;
out vec2 vTexCoord;

float rand(float x, float y){
  return fract(sin(dot(vec2(x, y) ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	vKeep = vertex[0].vKeep;
	vTexCoord = vertex[0].vTexCoord;
	vec4 dir = vec4(rand(gl_in[0].gl_Position.x, time) * 15.0f, rand(gl_in[0].gl_Position.y, time) * 15.0f, gl_in[0].gl_Position.z, 1.0);
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position + dir);
	EmitVertex();
	//gl_Position = vertex[0].fragPos;
	//EmitVertex();
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position - dir);
	EmitVertex();
	EndPrimitive();
}