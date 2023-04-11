#version 450
#extension GL_ARB_separate_shader_objects : enable
layout(set = 0, binding = 0) uniform UniformBufferObject {
	mat4 mvpMat;
	mat4 mMat;
	mat4 nMat;
} ubo;

layout(set = 0, binding = 1) uniform GlobalUniformBufferObject {
	float time;
} gubo;
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNorm;
layout(location = 2) in vec2 inUV;

layout(location = 0) out vec3 fragPos;
layout(location = 1) out vec3 fragNorm;
layout(location = 2) out vec2 fragUV;
void main() {
	float x = inPosition.x;
	float y = inPosition.y;
	float z = inPosition.z;
	float t = gubo.time;
	
	/***** Insert the code here to compute the correct value of y ****/

	/***** Leave it as is from this point on ****/
	
	vec3 vpos = vec3(x, y, z);
	
	gl_Position = ubo.mvpMat * vec4(vpos, 1.0);
	fragPos = (ubo.mMat * vec4(vpos, 1.0)).xyz;
	fragNorm = inNorm;
	fragUV = inUV;
}