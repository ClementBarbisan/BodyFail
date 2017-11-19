#version 150 core
layout(points) in;
layout(triangle_strip, max_vertices=60) out;

in Vertex
{
	vec2 vTexCoord;
	float vKeep;
} vertex[];

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform float lookalike;
out float vKeep;
out vec2 vTexCoord;
//out float zPos;
out vec4 Position_worldspace;
out vec4 Normal_cameraspace;
float size = 0.002;
uniform float time;

float rand(float x, float y){
  return fract(sin(dot(vec2(x, y) ,vec2(12.9898,78.233))) * 43758.5453);
}
/*
vec3 vertices[12];

void polygon(int i1, int i2, int i3)
{
	vec3 vertexNormal_modelspace = -normalize(cross(vertices[i2] - vertices[i1], vertices[i3] - vertices[i1]));
	Normal_cameraspace = (modelViewMatrix * vec4(vertexNormal_modelspace, 1.0));
	gl_Position = modelViewProjectionMatrix * vec4(vertices[i1].x, vertices[i1].y, vertices[i1].z, 1.0);
	EmitVertex();
	gl_Position = modelViewProjectionMatrix * vec4(vertices[i2].x, vertices[i2].y, vertices[i2].z, 1.0);
	EmitVertex();
	gl_Position = modelViewProjectionMatrix * vec4(vertices[i3].x, vertices[i3].y, vertices[i3].z, 1.0);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	vKeep = vertex[0].vKeep;
	if (vKeep == 0)
		return;
	//zPos = sqrt(( modelViewProjectionMatrix * gl_in[0].gl_Position).z) / 20.0;
	size = clamp((rand(float(gl_PrimitiveIDIn), time) / 100.0f) * (1.0 - lookalike), 0.002, 1.0);
	vec4 pos = vec4((-0.5 + rand(gl_in[0].gl_Position.x, time)) / (30.0 * lookalike), (-0.5 + rand(gl_in[0].gl_Position.y, time)) / (30.0 * lookalike), (-0.5 + rand(gl_in[0].gl_Position.z, time)) / (30.0 * lookalike), 1.0);
	vec4 zRand = vec4(((-1.0 + rand((gl_in[0].gl_Position+vec4(size,size / 2,size / 2,0.0)).x, time)) / 10.0) * (1.0 - lookalike),((-1.0 + rand((gl_in[0].gl_Position+vec4(size,size / 2,size / 2,0.0)).y, time)) / 10.0) * (1.0 - lookalike),((-1 + rand((gl_in[0].gl_Position+vec4(size,size / 2,size / 2,0.0)).z, time)) / 10.0) * (1.0 - lookalike),1.0);
	// Position of the vertex, in worldspace : M * position
	Position_worldspace = modelViewMatrix * (gl_in[0].gl_Position + pos);

	vTexCoord = vertex[0].vTexCoord;
	float Pi = 3.141592653589793238462643383279502884197;
	float phiaa  = 26.56505;
	float r = size;
	float phia = Pi*phiaa/180.0;
	float theb = Pi*36.0/180.0;
	float the72 = Pi*72.0/180;
	vertices[0] = vec3(0.0, 0.0, r) + pos.xyz + zRand.xyz + gl_in[0].gl_Position.xyz;
	vertices[11] = vec3(0.0, 0.0, r) + pos.xyz + zRand.xyz + gl_in[0].gl_Position.xyz;
	float the = 0.0;
	for(int i = 1; i < 6; i++)
	{
		vertices[i] = vec3(r*cos(the)*cos(phia), r*sin(the)*cos(phia), r*sin(phia)) + pos.xyz + gl_in[0].gl_Position.xyz;
		the = the + the72;
	}
	the = theb;
	for(int i = 6; i < 11; i++)
	{
		vertices[i] = vec3(r*cos(the)*cos(-phia), r*sin(the)*cos(-phia), r*sin(-phia)) + pos.xyz + gl_in[0].gl_Position.xyz;
		the = the + the72;
	}
	polygon(0,1,2);
	polygon(0,2,3);
	polygon(0,3,4);
	polygon(0,4,5);
	polygon(0,5,1);
	polygon(11,6,7);
	polygon(11,7,8);
	polygon(11,8,9);
	polygon(11,9,10);
	polygon(11,10,6);
	polygon(1,2,6);
	polygon(2,3,7);
	polygon(3,4,8);
	polygon(4,5,9);
	polygon(5,1,10);
	polygon(6,7,2);
	polygon(7,8,3);
	polygon(8,9,4);
	polygon(9,10,5);
	polygon(10,6,1);
}
*/

void main()
{
	vKeep = vertex[0].vKeep;
	if (vKeep == 0)
		return;
	// Position of the vertex, in worldspace : M * position
	Position_worldspace = modelViewMatrix * gl_in[0].gl_Position;
	
	//zPos = sqrt(( modelViewProjectionMatrix * gl_in[0].gl_Position).z) / 20.0;
	vec4 pos = vec4((rand(gl_in[0].gl_Position.x, time)) / (30.0 * lookalike), (rand(gl_in[0].gl_Position.y, time)) / (30.0 * lookalike), (rand(gl_in[0].gl_Position.z, time)) / (30.0 * lookalike), 1.0);
	vec4 zRand = vec4(((-1.0 + rand((gl_in[0].gl_Position+vec4(size,size / 2,size / 2,0.0)).x, time)) / 10.0) * (1.0 - lookalike),((-1.0 + rand((gl_in[0].gl_Position+vec4(size,size / 2,size / 2,0.0)).y, time)) / 10.0) * (1.0 - lookalike),((-1 + rand((gl_in[0].gl_Position+vec4(size,size / 2,size / 2,0.0)).z, time)) / 10.0) * (1.0 - lookalike),1.0);
	vec4 normalExp = vec4(0.0);
	size = clamp((rand(float(gl_PrimitiveIDIn), time) / 100.0f) * (1.0 - lookalike), 0.002, 1.0);
	vTexCoord = vertex[0].vTexCoord;
	//left
	//light = vec3(1.0);
	// Normal of the the vertex, in camera space
	vec3 vertexNormal_modelspace = -normalize(cross(vec3(size,-size / 2,-size / 2) - (vec3(size,size / 2,size / 2) + zRand.xyz), vec3(0.0,-size,0.0) - (vec3(size,size / 2,size / 2) + zRand.xyz)));
	Normal_cameraspace = (modelViewMatrix * vec4(vertexNormal_modelspace, 1.0));
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(size,size / 2,size / 2,0.0) + pos + zRand + normalExp);
	EmitVertex();
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(size,-size / 2,-size / 2,0.0) + pos + normalExp);
	EmitVertex();
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(0.0,0.0,size,0.0) + pos + normalExp);
	EmitVertex();
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(0.0,-size,0.0,0.0) + pos + normalExp);
	EmitVertex();
	EndPrimitive();
	//right
	//light = vec3(1.0);
	vertexNormal_modelspace = -normalize(cross(vec3(-size,size / 2,size / 2) - vec3(0.0,size,0.0), vec3(-size,-size / 2,-size / 2) - vec3(0.0,size,0.0)));
	Normal_cameraspace = (modelViewMatrix * vec4(vertexNormal_modelspace,1.0));
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(0.0,size,0.0,0.0) + pos + normalExp);
	EmitVertex();
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(-size,size / 2,size / 2,0.0) + pos + normalExp);
	EmitVertex();
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(0.0,0.0,-size,0.0) + pos + normalExp);
	EmitVertex();
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(-size,-size / 2,-size / 2,0.0) + pos + normalExp);
	EmitVertex();
	EndPrimitive();
	//top
	//light = vec3(1.0);
	vertexNormal_modelspace = -normalize(cross(vec3(0.0,size,0.0) - vec3(-size,size / 2,size / 2), (vec3(size,size / 2,size / 2) + zRand.xyz) - vec3(-size,size / 2,size / 2)));
	Normal_cameraspace = (modelViewMatrix * vec4(vertexNormal_modelspace, 1.0));
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(-size,size / 2,size / 2,0.0) + pos + normalExp);
	EmitVertex();
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(0.0,size,0.0,0.0) + pos + normalExp);
	EmitVertex();
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(0.0,0.0,size,0.0) + pos + normalExp);
	EmitVertex();
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(size,size / 2,size / 2,0.0) + pos + zRand + normalExp);
	EmitVertex();
	EndPrimitive();
	//bottom
	//light = vec3(0.2);
	vertexNormal_modelspace = -normalize(cross(vec3(0.0,-size,0.0) - vec3(-size,-size / 2,-size / 2), vec3(size,-size / 2,-size / 2) - vec3(-size,-size / 2,-size / 2)));
	Normal_cameraspace = (modelViewMatrix * vec4(vertexNormal_modelspace, 1.0));
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(-size,-size / 2,-size / 2,0.0) + pos + normalExp);
	EmitVertex();
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(0.0,-size,0.0,0.0) + pos + normalExp);
	EmitVertex();
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(0.0,0.0,-size,0.0) + pos + normalExp);
	EmitVertex();
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(size,-size / 2,-size / 2,0.0) + pos + normalExp);
	EmitVertex();
	EndPrimitive();
	//front
	//light = vec3(0.5);
	vertexNormal_modelspace = -normalize(cross(vec3(0.0,size,0.0) - (vec3(size,size / 2,size / 2) + zRand.xyz), vec3(0.0,0.0,-size) - (vec3(size,size / 2,size / 2) + zRand.xyz)));
	Normal_cameraspace = (modelViewMatrix * vec4(vertexNormal_modelspace, 1.0));
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(size,size / 2,size / 2,0.0) + pos + zRand + normalExp);
	EmitVertex();
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(0.0,size,0.0,0.0) + pos + normalExp);
	EmitVertex();
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(size,-size / 2,-size / 2,0.0) + pos + normalExp);
	EmitVertex();
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(0.0,0.0,-size,0.0) + pos + normalExp);
	EmitVertex();
	EndPrimitive();
	//back
	//light = vec3(0.5);
	vertexNormal_modelspace = -normalize(cross(vec3(-size,-size / 2,-size / 2) - vec3(-size,size / 2,size / 2), vec3(0.0,-size,0.0) - vec3(-size,size / 2,size / 2)));
	Normal_cameraspace = (modelViewMatrix * vec4(vertexNormal_modelspace, 1.0));
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(-size,size / 2,size / 2,0.0) + pos + normalExp);
	EmitVertex();
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(-size,-size / 2,-size / 2,0.0) + pos + normalExp);
	EmitVertex();
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(0.0,0.0,size,0.0) + pos + normalExp);
	EmitVertex();
	gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position+vec4(0.0,-size,0.0,0.0) + pos + normalExp);
	EmitVertex();
	EndPrimitive();
}