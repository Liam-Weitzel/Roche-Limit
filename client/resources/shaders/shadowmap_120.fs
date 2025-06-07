#version 120

// Input vertex attributes (from vertex shader)
varying vec3 fragPosition;
varying vec2 fragTexCoord;
//varying in vec4 fragColor;
varying vec3 fragNormal;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Input lighting values
uniform vec3 lightDir;
uniform vec4 lightColor;
uniform vec4 ambient;
uniform vec3 viewPos;

// Input shadowmapping values
uniform mat4 lightVP; // Light source view-projection matrix
uniform sampler2D shadowMap;

// Expanded Poisson disk samples for better quality
vec2 poissonDisk[16] = vec2[](
    vec2(-0.94201624, -0.39906216),
    vec2(0.94558609, -0.76890725),
    vec2(-0.094184101, -0.92938870),
    vec2(0.34495938, 0.29387760),
    vec2(-0.91588581, 0.45771432),
    vec2(-0.81544232, -0.87912464),
    vec2(-0.38277543, 0.27676845),
    vec2(0.97484398, 0.75648379),
    vec2(0.44323325, -0.97511554),
    vec2(0.53742981, -0.47373420),
    vec2(-0.26496911, -0.41893023),
    vec2(0.79197514, 0.19090188),
    vec2(-0.24188840, 0.99706507),
    vec2(-0.81409955, 0.91437590),
    vec2(0.19984126, 0.78641367),
    vec2(0.14383161, -0.14100790)
);

// Random number generator
float random(vec3 seed, int i) {
    vec4 seed4 = vec4(seed, i);
    float dot_product = dot(seed4, vec4(12.9898, 78.233, 45.164, 94.673));
    return fract(sin(dot_product) * 43758.5453);
}


// Customizable parameters as uniforms
uniform int shadowMapResolution;
uniform float SELF_SHADOW_INTENSITY;    // Controls darkness of faces facing away from light
uniform float SPECULAR_POWER;           // Controls shininess/specular highlight size
uniform float POISSON_DISK_SCALE;       // Controls shadow softness (higher = harder)
uniform float SHADOW_BIAS_FACTOR;       // Controls shadow bias primary factor
uniform float SHADOW_BIAS_MIN;          // Controls shadow bias minimum
uniform int POISSON_SAMPLES;            // Number of shadow samples
uniform float SHADOW_DARKNESS;          // How much each sample darkens
uniform float AMBIENT_DIVISION;         // Controls ambient light intensity
uniform float GAMMA;                    // Gamma correction value

void main()
{
    vec4 texelColor = texture2D(texture0, fragTexCoord);
    vec3 lightDot = vec3(0.0);
    vec3 normal = normalize(fragNormal);
    vec3 viewD = normalize(viewPos - fragPosition);
    vec3 specular = vec3(0.0);

    vec3 l = -lightDir;

    // Parameterized self-shadow intensity
    float NdotL = max(dot(normal, l) * SELF_SHADOW_INTENSITY, 0.0);
    lightDot += lightColor.rgb*NdotL;

    float specCo = 0.0;
    if (NdotL > 0.0) specCo = pow(max(0.0, dot(viewD, reflect(-(l), normal))), SPECULAR_POWER);
    specular += specCo;

    vec4 finalColor = (texelColor*((colDiffuse + vec4(specular, 1.0))*vec4(lightDot, 1.0)));

    // Shadow calculations
    vec4 fragPosLightSpace = lightVP*vec4(fragPosition, 1);
    fragPosLightSpace.xyz /= fragPosLightSpace.w;
    fragPosLightSpace.xyz = (fragPosLightSpace.xyz + 1.0)/2.0;
    vec2 sampleCoords = fragPosLightSpace.xy;
    float curDepth = fragPosLightSpace.z;

    // Parameterized shadow bias
    float bias = max(SHADOW_BIAS_FACTOR*(1.0 - dot(normal, l)), SHADOW_BIAS_MIN);
    
    // Stratified Poisson sampling
    float visibility = 1.0;
    for (int i = 0; i < POISSON_SAMPLES; i++) {
        float rnd = random(gl_FragCoord.xyz, i);
        int index = int(floor(rnd * 16.0));
        
        float sampleDepth = texture2D(shadowMap, sampleCoords + poissonDisk[index]/POISSON_DISK_SCALE).r;
        if (curDepth - bias > sampleDepth) {
            visibility -= SHADOW_DARKNESS;
        }
    }

    finalColor = mix(finalColor, vec4(0, 0, 0, 1), 1.0 - visibility);

    // Parameterized ambient lighting
    finalColor += texelColor*(ambient/AMBIENT_DIVISION)*colDiffuse;

    // Parameterized gamma correction
    finalColor = pow(finalColor, vec4(1.0/GAMMA));
    gl_FragColor = finalColor;
}
