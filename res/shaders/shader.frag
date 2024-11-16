#version 450

#define MAX_TEXTURES 16
#define DITHER_STEPS 6


const float threshold4x4[16] = float[16](
    0.0, 12.0, 3.0, 15.0,
    8.0, 4.0, 11.0, 7.0,
    2.0, 14.0, 1.0, 13.0,
    10.0, 6.0, 9.0, 5.0
);

vec3 bayerDither4x4(vec3 color, vec2 pos)
{
    int index = int(pos.x) % 4 + int(pos.y) % 4 * 4;
    
    float threshold = threshold4x4[index] / 16.;
    
    return floor(color * DITHER_STEPS + threshold) / DITHER_STEPS;
}

const vec3 ambient = vec3(0.3);

layout(set = 0, binding = 1) uniform sampler texSampler;
layout(set = 0, binding = 2) uniform texture2D textures[MAX_TEXTURES];

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec3 cameraPos;
layout(location = 3) flat in uint texIndex;
layout(location = 4) in vec2 fragTexCoord;
layout(location = 5) in float time;

layout(location = 0) out vec4 outColor;


const vec3 dayColor = vec3(1.f, 1.f, 1.f);
const vec3 nightColor = normalize(vec3(50, 50, 224));

void main() {
    float timeOfDay = /*mod((time / 120.f), 1.f)*/ 0.25f * 3.14159f;
    float sunAngle = timeOfDay * 2;
    
    vec3 lightDir = normalize(vec3(sin(sunAngle), cos(sunAngle), cos(sunAngle)));
//    vec3 lightDir = normalize(cameraPos - fragPos);
    
    vec3 lightColor = mix(dayColor, nightColor, sin(timeOfDay) * sin(timeOfDay));

    
    vec3 col = texture(sampler2D(textures[texIndex], texSampler), fragTexCoord).rgb;

    vec3 diffuse = (max(dot(normalize(cross(dFdy(fragPos), dFdx(fragPos))), lightDir), 0.0) + ambient) * col * lightColor;
    
    float distanceFactor = clamp(distance(fragPos, cameraPos) / 6.f, 0.0, 1.0);
    diffuse *= 1.f - distanceFactor;

    float grain = fract(sin(dot(fragTexCoord.xy, vec2(12.9898, 78.233))) * 43758.5453) * 0.03;
    
    vec3 finalColor = bayerDither4x4(1.5f * diffuse, gl_FragCoord.xy) + grain;
    
    outColor = vec4(finalColor, 1.0);
}
