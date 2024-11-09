#version 450
#define MAX_TEXTURES 16

// Enhanced dithering pattern for more visible pixel art effect
const float threshold4x4[16] = float[16](
    0.0, 12.0, 3.0, 15.0,
    8.0, 4.0, 11.0, 7.0,
    2.0, 14.0, 1.0, 13.0,
    10.0, 6.0, 9.0, 5.0
);

// Color quantization function
vec3 quantizeColor(vec3 color, float levels) {
    return floor(color * levels) / levels;
}

vec3 bayerDither4x4(vec3 color, int x, int y) {
    int index = x + y * 4;
    
    float threshold = threshold4x4[index] / 16.;
    
//    float brightness = dot(vec3(0.299, 0.587, 0.114), color); // Updated color weights
//    return step(threshold, brightness);
    
    return floor(color * 3 + threshold) / 3;
}

const float ambientStrength = 0.3; // Increased ambient for better visibility
const vec3 lightDir = vec3(0.5, 1.0, 0.0);

layout(set = 0, binding = 1) uniform sampler texSampler;
layout(set = 0, binding = 2) uniform texture2D textures[MAX_TEXTURES];

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in uint texIndex;
layout(location = 3) in vec3 fragNormal;
layout(location = 4) in float time;
layout(location = 5) out vec2 screenResolution;

layout(location = 0) out vec4 outColor;

void main() {
    // Sample texture and quantize to fewer colors
    vec3 col = texture(sampler2D(textures[texIndex], texSampler), fragTexCoord).rgb;
    
    // Calculate pixel position for dithering
    int x = int(gl_FragCoord.x) % 4;
    int y = int(gl_FragCoord.y) % 4;
    
    // Enhanced lighting
    vec3 ambient = vec3(ambientStrength);
    vec3 norm = normalize(fragNormal);
    vec3 lightDirection = normalize(lightDir);
    float diff = max(dot(norm, lightDirection), 0.0);
    
    // Add slight noise for texture
    float grain = fract(sin(dot(fragTexCoord.xy, vec2(12.9898, 78.233))) * 43758.5453) * 0.03;
//    
//    // Combine lighting with enhanced contrast
    vec3 diffuse = pow(1.5 * (diff + ambient) * col, vec3(1));
//     // Further reduce colors for lighting
//    
//    // Apply dithering with stronger effect
//    vec3 bayer = vec3(bayerDither4x4(diffuse, x, y));
//    
//    // Final color with enhanced contrast and gamma correction
    vec3 finalColor = bayerDither4x4(diffuse, x, y) + grain;
//
//    vec3 finalColor = quantizeColor(bayerDither4x4(col, x, y), 8.);
    
    outColor = vec4(finalColor, 1.0);
//    if (time < 3.14)
//    {
//        outColor = vec4(finalColor, 1.0);
//    }
//    else
//    {
//        outColor = vec4(mix(finalColor, fragNormal, 0.5 * (cos(time) + 1)), 1.0);
//    }
}
