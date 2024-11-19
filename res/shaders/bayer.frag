#version 450

#define MAX_TEXTURES 4
#define DITHER_STEPS 6

#define GRAIN true


const float threshold4x4[16] = float[16](
    0.0, 12.0, 3.0, 15.0,
    8.0, 4.0, 11.0, 7.0,
    2.0, 14.0, 1.0, 13.0,
    10.0, 6.0, 9.0, 5.0
);
vec3 bayerDither4x4(vec3 color, vec2 pos)
{
    int index = (int(pos.x) & 3) + ((int(pos.y) & 3) << 2);
    
    float threshold = threshold4x4[index] * (0.0625f);
    
    return floor(color * DITHER_STEPS + threshold) * (1.0 / DITHER_STEPS);
}

const vec3 ambient = vec3(0.1f);

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
const vec3 nightColor = normalize(vec3(1, 1, 1));

void main() {
    float timeOfDay = /*mod((time / 120.f), 1.f)*/ 2.5f;
    float sunAngle = timeOfDay * 2;
    
    vec3 lightDir = normalize(vec3(sin(sunAngle), cos(sunAngle), cos(sunAngle)));
    vec3 lightColor = mix(dayColor, nightColor, sin(timeOfDay) * sin(timeOfDay));
    
    vec3 col = texture(sampler2D(textures[texIndex], texSampler), fragTexCoord).rgb;

    vec3 normal = normalize(cross(dFdy(fragPos), dFdx(fragPos)));
    vec3 diffuse = (max(dot(normal, lightDir), 0.0) + ambient) * col * lightColor;
    
    float distanceFactor = clamp(distance(fragPos.xz, cameraPos.xz) * 0.16f, 0.0, 1.0);
    diffuse *= 1.f - distanceFactor;
    
    vec3 finalColor = bayerDither4x4(diffuse * 2, gl_FragCoord.xy);
    
    float grain = 0.01f;
    
    if (GRAIN)
    {
        grain = fract(sin(dot(fragTexCoord.xy + sin(time), vec2(12.9898f, 78.233f))) * 43758.5453f) * 0.03f;
    }
    
    outColor = vec4(finalColor + grain, 1.0);
}
