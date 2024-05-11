#version 450

layout(location = 0) in vec3 fragColor;
// Remove the fragNormal input variable, as we'll be using the normal sampled from the normal map

layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;
layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform sampler2D normalMapSampler; 

void main() 
{
    const vec3 lightDirection = normalize(vec3(-0.5, -0.5, -0.5));

    // Sample the normal from the normal map
    vec3 sampledNormal = texture(normalMapSampler, fragTexCoord).rgb * 2.0 - 1.0; // Convert from [0, 1] to [-1, 1] range

    // Ensure the sampled normal is normalized
    vec3 normal = normalize(sampledNormal);

    // Calculate the dot product between the normal and the light direction
    float diff = max(dot(normal, lightDirection), 0.2);

    // Sample the texture
    vec4 texColor = texture(texSampler, fragTexCoord);

    // Simple diffuse lighting
    vec3 diffuse = diff * vec3(1.0); // Assuming white light

    // Output color
    outColor = vec4(texColor.rgb * diffuse, texColor.a); // Use diffuse color instead of fragColor
}