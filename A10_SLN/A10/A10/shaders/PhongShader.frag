#version 450#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;layout(location = 1) in vec3 fragNorm;layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform GlobalUniformBufferObject {
	float time;} gubo;

void main() {
	vec3 Norm = fragNorm;		float y = fragPos.y;	float t = gubo.time;	float r = 0.0f, g = 0.0f, b = 0.0f;
	/***** Insert the code here to compute the correct value of r, g and b ****/	/***** Leave it as is from this point on ****/		outColor = vec4(r, g, b, 1.0);}