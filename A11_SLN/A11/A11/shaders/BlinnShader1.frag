#version 450#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;layout(location = 1) in vec3 fragNorm;layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;layout(binding = 1) uniform sampler2D tex;

layout(binding = 2) uniform GlobalUniformBufferObject {
	vec3 lightDir;
	vec4 lightColor;
	vec3 eyePos;
} gubo;

void main() {
	vec3 Norm = normalize(fragNorm);
	vec3 EyeDir = normalize(gubo.eyePos - fragPos);	// replace the following lines with the code to implement a directional light model	// with the light color in gubo.lightColor, and the direction in gubo.lightDir.	vec3 lightDir = vec3(0.0,1.0,0.0);	vec3 lightColor = vec3(1.0,0.15,0.2);

	vec3 Diffuse = texture(tex, fragUV).rgb * 0.95f * clamp(dot(Norm, lightDir),0.0,1.0);	vec3 Specular = vec3(pow(clamp(dot(Norm, normalize(lightDir + EyeDir)),0.0,1.0), 160.0f));
	vec3 Ambient = texture(tex, fragUV).rgb * 0.05f;	
	outColor = vec4(clamp((Diffuse + Specular) * lightColor.rgb + Ambient,0.0,1.0), 1.0f);
}