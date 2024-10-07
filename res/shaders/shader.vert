#version 450

layout(binding = 0) uniform UniformBufferObject
{
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in uint inTexIndex;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out flat uint texIndex;


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
    gl_Position = snap(ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0), vec2(160.0, 120.0));
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    texIndex = inTexIndex;
}
