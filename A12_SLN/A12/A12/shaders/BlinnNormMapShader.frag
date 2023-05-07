#version 450#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;layout(location = 1) in vec3 fragNorm;layout(location = 2) in vec4 fragTan;layout(location = 3) in vec2 fragUV;

layout(location = 0) out vec4 outColor;layout(binding = 1) uniform sampler2D tex;
layout(binding = 3) uniform sampler2D normMap;layout(binding = 4) uniform sampler2D matMap;
layout(binding = 2) uniform GlobalUniformBufferObject {
	vec3 lightDir;
	vec4 lightColor;
	vec3 eyePos;
} gubo;vec3 BRDF(vec3 V, vec3 N, vec3 L, vec3 Md, vec3 Ms, float gamma) {	//vec3 V  - direction of the viewer	//vec3 N  - normal vector to the surface	//vec3 L  - light vector (from the light model)	//vec3 Md - main color of the surface	//vec3 Ms - specular color of the surface	//float gamma - Exponent for power specular term		return vec3(0,0,1);}
void main() {
	vec3 Norm = normalize(fragNorm);	vec3 Tan = normalize(fragTan.xyz - Norm * dot(fragTan.xyz, Norm));	vec3 Bitan = cross(Norm, Tan) * fragTan.w;	mat3 tbn = mat3(Tan, Bitan, Norm);	vec4 nMap = texture(normMap, fragUV);	vec3 N = normalize(tbn * (nMap.rgb * 2.0 - 1.0));	vec3 albedo = texture(tex, fragUV).rgb;	vec4 MRAO = texture(matMap, fragUV);	float roughness = MRAO.g;	float pex = 1000.f * (1.0 - roughness) * (1.0 - roughness);	float ao = MRAO.b;	float metallic = MRAO.r;		vec3 L = gubo.lightDir;	vec3 lightColor = gubo.lightColor.rgb;	vec3 V = normalize(gubo.eyePos - fragPos);	vec3 DiffSpec = BRDF(V, N, L, albedo, 0.9f * vec3(metallic), pex);	vec3 Ambient = albedo * 0.05f * ao;		outColor = vec4(clamp(0.95 * DiffSpec * lightColor.rgb + Ambient,0.0,1.0), 1.0f);}