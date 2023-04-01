#version 450#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;layout(location = 1) in vec3 fragNorm;layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform GlobalUniformBufferObject {
	vec3 lightDir;
	vec4 lightColor;
	vec3 eyePos;
} gubo;

void main() {
	vec3 Norm = fragNorm;

	vec3 EyeDir = normalize(gubo.eyePos - fragPos);
	Norm = dot(Norm, gubo.lightDir) > 0 ? Norm : -Norm;

	vec3 Diffuse = (vec3(1.0f,0.9f,0.9f) * (dot(Norm, gubo.lightDir)) - vec3(0.3f,0.3f,0.3f)) / 0.7f;
	
	outColor = vec4(Diffuse * gubo.lightColor.rgb, 1.0f);
}