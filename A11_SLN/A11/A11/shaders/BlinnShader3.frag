#version 450#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;layout(location = 1) in vec3 fragNorm;layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;layout(binding = 1) uniform sampler2D tex;

layout(binding = 2) uniform GlobalUniformBufferObject {
	vec3 lightPos;	vec3 lightDir;
	vec4 lightColor;
	vec3 eyePos;
} gubo;
const float beta = 2.0f;const float g = 1.5;const float cosout = 0.85;const float cosin  = 0.95;
void main() {
	vec3 Norm = normalize(fragNorm);	vec3 EyeDir = normalize(gubo.eyePos - fragPos);		// replace the following lines with the code to implement a spot light model (based on point lights)	// with the light color in gubo.lightColor, the position in gubo.lightPos,	// and the direction in gubo.lightDir.	// The exponent of the decay is in constant variable beta, and the base distance	// is in constant g, the cosines of the inner and outer angle are respectively	// in constants cosin and cosout	vec3 lightDir = vec3(0.0,1.0,0.0);	vec3 lightColor = vec3(1.0,0.15,0.2);	vec3 Diffuse = texture(tex, fragUV).rgb * 0.995f * clamp(dot(Norm, lightDir),0.0,1.0);	vec3 Specular = vec3(pow(clamp(dot(Norm, normalize(lightDir + EyeDir)),0.0,1.0), 160.0f));	vec3 Ambient = texture(tex, fragUV).rgb * 0.005f;		outColor = vec4(clamp((Diffuse + Specular) * lightColor.rgb + Ambient,0.0,1.0), 1.0f);}