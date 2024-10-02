#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

const float threshold8x8[64] = float[64](
    0., 48., 12., 60., 3., 51., 15., 63.,
    32., 16., 44., 28., 35., 19., 47., 31.,
    8., 56., 4., 52., 11., 59., 7., 55.,
    40., 24., 36., 20., 43., 27., 39., 23.,
    2., 50., 14., 62., 1., 49., 13., 61.,
    34., 18., 46., 30., 33., 17., 45., 29.,
    10., 58., 6., 54., 9., 57., 5., 53.,
    42., 26., 38., 22., 41., 25., 37., 21.);

const float threshold4x4[16] = float[16](
    0.0, 8.0, 2.0, 10.0,
    12.0, 4.0, 14.0, 6.0,
    3.0, 11.0, 1.0, 9.0,
    15.0, 7.0, 13.0, 5.0
);

float bayerDither8x8(vec3 color, int x, int y)
{
    int index = x + y * 8;
    float threshold = threshold8x8[index] / 64.0;
    float brightness = dot(vec3(0.2), color);
    return step(threshold, brightness);
}

float bayerDither4x4(vec3 color, int x, int y)
{
    int index = x + y * 4;
    float threshold = threshold4x4[index] / 16.0;
    float brightness = dot(vec3(0.2126, 0.7152, 0.0722), color);
    return step(threshold, brightness);
}

void main()
{
    vec3 col = texture(texSampler, fragTexCoord).rgb;
    
    int stippleSize = 1;
    
    bool use4x4 = true;
    
    float dither;
    if (use4x4) {
        int x = int(gl_FragCoord.x / stippleSize) % 4;
        int y = int(gl_FragCoord.y / stippleSize) % 4;
        dither = bayerDither4x4(col, x, y);
    } else {
        int x = int(gl_FragCoord.x / stippleSize) % 8;
        int y = int(gl_FragCoord.y / stippleSize) % 8;
        dither = bayerDither8x8(col, x, y);
    }
    

//     outColor = vec4(mix(vec3(0.0), col, dither), 1.0);
    outColor = vec4(dither);
}
