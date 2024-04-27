#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;
layout(binding = 1) uniform sampler2D texSampler;

void main() 
{
    const vec3 lightDirection = normalize(vec3(-0.5, -0.5, -0.5));

    // Calculate the dot product between the normal and the light direction
    float diff = max(dot(fragNormal, lightDirection), 0.2);

    // Sample the texture
    vec4 texColor = texture(texSampler, fragTexCoord);

    // Simple diffuse lighting
    vec3 diffuse = diff * vec3(1.0); // Assuming white light

    // Output color
    outColor = vec4(texColor.rgb * diffuse, texColor.a); // Use diffuse color instead of fragCaaaaaaaaaalor
}
