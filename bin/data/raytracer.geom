#version 150 core
layout(lines) in;
layout(line_strip, max_vertices=2) out;

uniform float time;
uniform float lookalike;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
out vec4 Position_worldspace;

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
	vKeep = vertex[0].vKeep == 1.0 ? vertex[1].vKeep : 0.0;
	vec4 pos = vec4((-0.5 + rand(gl_in[0].gl_Position.x, time)) / (30.0 * lookalike), (-0.5 + rand(gl_in[0].gl_Position.y, time)) / (30.0 * lookalike), (-0.5 + rand(gl_in[0].gl_Position.z, time)) / (30.0 * lookalike), 1.0);
	Position_worldspace = modelViewMatrix * (gl_in[0].gl_Position + pos);
	vTexCoord = vertex[0].vTexCoord;
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position + pos);
	EmitVertex();
	pos = vec4((rand(gl_in[0].gl_Position.x, time)) / (20.0 * lookalike), (rand(gl_in[0].gl_Position.y, time)) / (20.0 * lookalike), (rand(gl_in[0].gl_Position.z, time)) / (20.0 * lookalike), 1.0);
	Position_worldspace = modelViewMatrix * (gl_in[0].gl_Position + pos);
	vTexCoord = vertex[1].vTexCoord;
	gl_Position = modelViewProjectionMatrix * (gl_in[1].gl_Position + pos);
	EmitVertex();
	EndPrimitive();
}