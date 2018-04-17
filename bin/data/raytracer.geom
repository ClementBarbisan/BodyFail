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
	int id;
} vertex[];

out float vKeep;
out vec2 vTexCoord;

float rand(float x, float y){
  return fract(sin(dot(vec2(x, y) ,vec2(12.9898,78.233))) * 43758.5453);
}

float EaseInOut(float t)
{
	return (t*t);
    if(t > 0.5f)
        return 2.0f * (t * t);
    t -= 0.5f;
    return 2.0f * t * (1.0f - t) + 0.5;
}


void main()
{
	vKeep = vertex[0].vKeep == 1.0 ? vertex[1].vKeep : 0.0;
	if (distance(gl_in[0].gl_Position,gl_in[1].gl_Position) < 0.2)
		vKeep = 0.0;
	if (vKeep == 0.0)
		return;
	float value = EaseInOut(lookalike);
	float computeTime = time * (2 + 2 * (1.0 - value));
	vec4 pos = vec4(sin(computeTime + gl_in[0].gl_Position.z + vertex[0].id) * cos(computeTime + gl_in[0].gl_Position.y +  vertex[0].id) / (50.0 * (value)), sin(computeTime + gl_in[0].gl_Position.x +  vertex[0].id) * sin(computeTime + gl_in[0].gl_Position.z +  vertex[0].id) / (50.0 * (value)), sin(computeTime + gl_in[0].gl_Position.x +  vertex[0].id) * cos(computeTime + gl_in[0].gl_Position.y +  vertex[0].id) / (50.0 * (value)), 1.0);
	vTexCoord = vertex[0].vTexCoord;
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position + pos);
	EmitVertex();
	pos = vec4(sin(computeTime + gl_in[1].gl_Position.z + vertex[1].id) * cos(computeTime + gl_in[1].gl_Position.y +  vertex[1].id) / (50.0 * (value)), sin(computeTime + gl_in[1].gl_Position.x +  vertex[1].id) * sin(computeTime + gl_in[1].gl_Position.z +  vertex[1].id) / (50.0 * (value)), sin(computeTime + gl_in[1].gl_Position.x +  vertex[1].id) * cos(computeTime + gl_in[1].gl_Position.y +  vertex[1].id) / (50.0 * (value)), 1.0);
	vTexCoord = vertex[1].vTexCoord;
	gl_Position = modelViewProjectionMatrix * (gl_in[1].gl_Position + pos);
	EmitVertex();
	EndPrimitive();
}