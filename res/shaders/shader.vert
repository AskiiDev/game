#version 450

// Uniform buffer for global view and projection matrices
layout(binding = 0) uniform UniformBufferObject
{
    mat4 view;
    mat4 proj;
    vec2 screenResolution;
    vec3 cameraPos;
    float time;
} ubo;

// Push constants for per-object model matrix
layout(push_constant) uniform PushConstants
{
    mat4 modelMatrix;
} pc;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in uint inTexIndex;
//layout(location = 4) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPos;
layout(location = 2) out vec3 cameraPos;
layout(location = 3) flat out uint texIndex;
layout(location = 4) out vec2 fragTexCoord;
layout(location = 5) out float time;


vec4 snap(vec4 vertex, vec2 res)
{
    vec4 snapped = vertex;
    snapped.xyz = vertex.xyz / vertex.w;
    snapped.xy = floor(res * snapped.xy) / res;
    snapped.xyz *= vertex.w;
    return snapped;
}

void main()
{
    // Apply model matrix from push constants
//    gl_Position = ubo.proj * ubo.view * pc.modelMatrix * vec4(inPosition, 1.0);
//    gl_Position = snap(ubo.proj * ubo.view * pc.modelMatrix * vec4(inPosition, 1.0), vec2(256.0, 224.0));
    
//    fragPosition = (ubo.proj * ubo.view * pc.modelMatrix * vec4(inPosition, 1.0));
    gl_Position = (ubo.proj * ubo.view * pc.modelMatrix * vec4(inPosition, 1.0));
    fragPos = vec3(pc.modelMatrix * vec4(inPosition, 1.0));
    cameraPos = ubo.cameraPos;
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    texIndex = inTexIndex;
    time = ubo.time;
}
