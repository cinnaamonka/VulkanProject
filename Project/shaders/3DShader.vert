#version 450

layout(set=0,binding = 0) uniform UniformBufferObject {
    mat4 proj;
    mat4 view; 
} vp;

layout(push_constant) uniform PushConstants {
    mat4 model; 
} mesh;


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec3 fragColor;

void main() {
    gl_Position = vp.proj * vp.view * mesh.model*vec4(inPosition,1);
    vec4 tNormal =  mesh.model*vec4(inNormal,0);
    fragNormal = normalize(tNormal.xyz); // interpolation of normal attribute in fragment shader.
    fragColor = inColor; // interpolation of color attribute in fragment shader.
}