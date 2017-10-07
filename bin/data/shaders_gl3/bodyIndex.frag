#version 150

uniform sampler2DRect uColorTex;
uniform sampler2DRect depthTex;
uniform int uWidth;

in float vKeep;
in vec2 vTexCoord;
in vec3 light;
in float zPos;

out vec4 vFragColor;

void main()
{
	if (vKeep == 0.0) {
		discard;
	}
	float ndcDepth = (1.5 * zPos - gl_DepthRange.near - gl_DepthRange.far) / (gl_DepthRange.far - gl_DepthRange.near);
	//float clipDepth = ndcDepth / gl_FragCoord.w;
	//vFragColor = texture(uColorTex, vTexCoord) * vec4(light, 1.0);
	//vFragColor = vec4(0.1 + clamp((vTexCoord.x / uWidth), 0.0, 1.0), 0.1 + clamp((vTexCoord.x / uWidth), 0.0, 1.0), 1.0, 1.0) * vec4(light, 1.0);
	//vFragColor = vec4(light, 1.0) * vec4((vTexCoord.x / uWidth) * 0.5, (vTexCoord.x / uWidth) * 0.5, (vTexCoord.x / uWidth) * 0.5, 1.0);
	vec4 rgb = texture(depthTex, vTexCoord);
	vFragColor = vec4(1.0 - clamp(smoothstep(0, 1, pow(ndcDepth, 0.9)), 0, 1)) * vec4(1.0 - clamp(smoothstep(0, 1, pow(rgb.r, 0.9)), 0, 1), 1.0 - clamp(smoothstep(0, 1, pow(rgb.g, 0.9)), 0, 1), 1.0 - clamp(smoothstep(0, 1, pow(rgb.b, 0.9)), 0, 1), 1.0);
}