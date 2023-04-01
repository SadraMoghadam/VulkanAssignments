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
	float m_real = 0.0f;	float m_img = 0.0f;	float temp;	int i;		for(i = 0; i < 16; i++) {		if(m_real * m_real + m_img * m_img > 4.0) {			break;		}		temp = m_real * m_real - m_img * m_img + fragUV.x;		m_img = 2.0 * m_real * m_img + fragUV.y;		m_real = temp;	}	outColor = vec4(float(i % 5) / 5.0,					float(i % 10) / 10.0,					float(i) / 15.0,					1.0);
}