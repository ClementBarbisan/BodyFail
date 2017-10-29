#version 150

uniform sampler2DRect uColorTex;
uniform sampler2DRect depthTex;
uniform int uWidth;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;

in float vKeep;
in vec2 vTexCoord;
//in float zPos;
in vec4 Position_worldspace;
in vec4 Normal_cameraspace;
in vec4 LightDirection_cameraspace;

out vec4 vFragColor;

void main()
{
	if (vKeep == 0.0) {
		discard;
	}

	// Light emission properties
	// You probably want to put them as uniforms
	vec4 LightColor = vec4(1,1,1,1);
	float LightPower = 300.0f;
	
	// Material properties
	vec4 MaterialSpecularColor = vec4(0.3, 0.3, 0.3, 1.0);

	// Distance to the light
	float distance = length( vec4(-1, 1, 0, 1) - Position_worldspace );

	// Normal of the computed fragment, in camera space
	vec4 n = normalize( Normal_cameraspace );
	// Direction of the light (from the fragment to the light)
	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec4 LightPosition_cameraspace = modelViewMatrix * vec4(-1, 1, 0, 1);
	vec4 l = normalize( LightPosition_cameraspace + (vec4(0.0) - Position_worldspace));
	float cosTheta = clamp( dot( n,l ), 0,1 );
	
	vec4 E = normalize(vec4(0.0) - Position_worldspace);
	vec4 R = reflect(-l,n);
	float cosAlpha = clamp( dot( E,R ), 0,1 );

	float ndcDepth = (gl_FragCoord.z - gl_DepthRange.near - gl_DepthRange.far) / (gl_DepthRange.far - gl_DepthRange.near);
	vec4 rgb = texture(depthTex, vTexCoord);
	vFragColor =
		vec4(1.0 - clamp(smoothstep(0, 1, pow(ndcDepth, 0.9)), 0, 1)) * 
		vec4(1.0 - clamp(smoothstep(0, 1, pow(rgb.r, 0.9)), 0, 1), 1.0 - clamp(smoothstep(0, 1, pow(rgb.g, 0.9)), 0, 1), 1.0 - clamp(smoothstep(0, 1, pow(rgb.b, 0.9)), 0, 1), 1.0)
		* LightColor * LightPower * cosTheta / (distance * 3.0) +
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance * 3.0) + vec4(0,0,0,1);
}