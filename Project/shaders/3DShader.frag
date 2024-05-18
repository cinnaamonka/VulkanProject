#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 fragTangent;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform sampler2D normalMapSampler; 
layout(binding = 3) uniform sampler2D specularMapSampler; 
layout(binding = 4) uniform sampler2D roughnessMapSampler; 

void main() 
{
    const vec3 lightDirection = normalize(vec3(-0.5, -0.5, -0.5));

    vec4 ambientOcclusion = vec4(0.05f, 0.05f, 0.05f, 1.0f);
    float sampledSpecularStrength = 2.0f;
    float diffuseStrengthKd = 7.0f;
    float specularIntensityFlag = 20.f;

    // Normal mapping
    vec3 binormal = cross(fragNormal, fragTangent);
    mat3 tangentSpaceAxis = mat3(fragTangent, binormal, fragNormal);
    vec3 sampledNormalColor = texture(normalMapSampler, fragTexCoord).xyz * 2.0 - 1.0;
    vec3 resultNormal = normalize(tangentSpaceAxis * sampledNormalColor);

    // Cosine angle between light direction and normal
    float cosTheta = max(dot(resultNormal, lightDirection), 0.0);

    // Diffuse reflection
    vec4 sampleDiffuse = texture(texSampler, fragTexCoord);
    vec3 lamberDiffuse = sampleDiffuse.xyz * cosTheta * diffuseStrengthKd; 

    // Specular reflection
    vec3 reflection = reflect(normalize(lightDirection), resultNormal);
    float cosAlpha = max(dot(reflection, normalize(vec3(0.0, 0.0, 1.0))), 0.0); 
    vec3 sampledSpecular = texture(specularMapSampler, fragTexCoord).rgb;
    vec3 sampledRoughness = texture(roughnessMapSampler, fragTexCoord).rgb; 

    // Calculate specular intensity using the roughness value
    float specularIntensity = pow(cosAlpha, sampledRoughness.r * specularIntensityFlag);

    // Combine specular color and intensity
    vec3 specularColor = sampledSpecularStrength * specularIntensity * sampledSpecular;

    // Final color
    outColor = vec4((lamberDiffuse + specularColor + ambientOcclusion.rgb) * cosTheta, 1.0);
}
