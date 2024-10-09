#version 450
#define MAX_TEXTURES 16

layout(set = 0, binding = 1) uniform sampler texSampler;
layout(set = 0, binding = 2) uniform texture2D textures[MAX_TEXTURES];

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in uint texIndex;

layout(location = 0) out vec4 outColor;

const float threshold4x4[16] = float[16](
    0.0, 8.0, 2.0, 10.0,
    12.0, 4.0, 14.0, 6.0,
    3.0, 11.0, 1.0, 9.0,
    15.0, 7.0, 13.0, 5.0
);

float bayerDither4x4(vec3 color, int x, int y)
{
    int index = x + y * 4;
    float threshold = threshold4x4[index] / 16.0;
    float brightness = dot(vec3(0.2126, 0.7152, 0.0722), color);
    return step(threshold, brightness);
}


void main()
{
//    float pixelationFactor = 512.0;

    // Get the texture coordinates and adjust them to achieve the pixelation effect
//    vec2 pixelatedCoords = floor(fragTexCoord * pixelationFactor) / pixelationFactor;

    // Sample the texture using the pixelated coordinates
    vec3 col = texture(sampler2D(textures[texIndex], texSampler), fragTexCoord).rgb;
//    outColor = vec4(col, 1.0);
    
    float dither;
    
    int x = int(gl_FragCoord.x) % 4;
    int y = int(gl_FragCoord.y) % 4;
    
    vec3 bayer = vec3(bayerDither4x4(col.rgb, x, y));
    
    outColor = vec4(bayer * pow(col, vec3(0.4)), 1.0);
    outColor *= 2;
//    outColor = bayer;
}
