#version 150

const vec3 LumCoeff = vec3 (0.2125, 0.7154, 0.0721);

uniform sampler2DRect uTexColor;
uniform sampler2DRect depthTex;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;

in float vKeep;
in vec2 vTexCoord;
in vec4 Position_worldspace;
in vec4 LightDirection_cameraspace;

out vec4 vFragColor;

void main()
{
	if (vKeep == 0.0)
		discard;
	// Light emission properties
	// You probably want to put them as uniforms
	//vec4 LightColor = vec4(1,1,1,1);
	//float LightPower = 300.0f;
	
	// Material properties
	//vec4 MaterialSpecularColor = vec4(0.3, 0.3, 0.3, 1.0);

	// Distance to the light
	//float distance = length( vec4(-1, 1, 0, 1) - Position_worldspace );

	// Normal of the computed fragment, in camera space
	//vec4 n = normalize( (vec4(0.0) - Position_worldspace) );
	// Direction of the light (from the fragment to the light)
	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	//vec4 LightPosition_cameraspace = modelViewMatrix * vec4(-1, 1, 0, 1);
	//vec4 l = normalize( LightPosition_cameraspace + (vec4(0.0) - Position_worldspace));
	//float cosTheta = clamp( dot( n,l ), 0,1 );
	
	//vec4 E = normalize(vec4(0.0) - Position_worldspace);
	//vec4 R = reflect(-l,n);
	//float cosAlpha = clamp( dot( E,R ), 0,1 );
	vec4 rgb = texture(depthTex, vTexCoord);

	//float ndcDepth = ((gl_FragDepth * 1.5) - (gl_DepthRange.near - gl_DepthRange.far)) / (gl_DepthRange.far - gl_DepthRange.near);
	//vFragColor =
	//	vec4(ndcDepth) 
		//* vec4(1.0 - clamp(smoothstep(0, 1, pow(rgb.r, 0.9)), 0, 1), 1.0 - clamp(smoothstep(0, 1, pow(rgb.g, 0.9)), 0, 1), 1.0 - clamp(smoothstep(0, 1, pow(rgb.b, 0.9)), 0, 1), 1.0)
	//	* LightColor * LightPower / (distance * 2.5) +
		//MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance * 2.5);

	//vec3 intensity     = vec3 (dot(vFragColor.rgb, LumCoeff));
    //vec3 color         = mix(intensity, vFragColor.rgb, vec3(1.0));
    //color              = mix(vec3(0.62), color, vec3(3.0));
	//color *= 1.5;
    vFragColor       = vec4 (0.75);
}