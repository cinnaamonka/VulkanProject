#version 450

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main() {
    const vec3 lightDirection = normalize(vec3(0.0, -1.0, -1.0));

    // Calculate the dot product between the normal and the light direction
    float diff = max(dot(fragNormal, lightDirection), 0.2);

    // Simple diffuse lighting
    vec3 diffuse = diff * fragColor; // Assuming white light

    // Output color
    outColor = vec4(fragColor, 1.0);
}