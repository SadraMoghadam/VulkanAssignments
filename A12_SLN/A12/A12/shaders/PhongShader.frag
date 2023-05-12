#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;


layout(binding = 1) uniform sampler2D tex;

layout(binding = 2) uniform GlobalUniformBufferObject {
	vec3 lightDir;
	vec4 lightColor;
	vec3 eyePos;
} gubo;

vec3 BRDF(vec3 V, vec3 N, vec3 L, vec3 Md, vec3 Ms, float gamma) {
	//vec3 V  - direction of the viewer
	//vec3 N  - normal vector to the surface
	//vec3 L  - light vector (from the light model)
	//vec3 Md - main color of the surface
	//vec3 Ms - specular color of the surface
	//float gamma - Exponent for power specular term
	vec3 Rlx = 2 * N * dot(L, N) - L;
	vec3 specular = Ms * pow(clamp(dot(V, Rlx), 0.0, 1.0), gamma);
	vec3 diffuse = Md * max(dot(L, N), 0.0);
	return specular + diffuse;
}

void main() {
	vec3 Norm = normalize(fragNorm);
	vec3 EyeDir = normalize(gubo.eyePos - fragPos);
	
	vec3 lightDir = gubo.lightDir;
	vec3 lightColor = gubo.lightColor.rgb;

	vec3 DiffSpec = BRDF(EyeDir, Norm, lightDir, texture(tex, fragUV).rgb, vec3(1.0f), 160.0f);
	vec3 Ambient = texture(tex, fragUV).rgb * 0.05f;
	
	outColor = vec4(clamp(0.95 * (DiffSpec) * lightColor.rgb + Ambient,0.0,1.0), 1.0f);
}