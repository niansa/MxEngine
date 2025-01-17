#include "Library/shader_utils.glsl"

in vec2 TexCoord;
out vec4 OutColor;

uniform sampler2D albedoTex;
uniform sampler2D normalTex;
uniform sampler2D materialTex;
uniform sampler2D depthTex;

struct Camera
{
    mat4 invViewProjMatrix;
    mat4 viewProjMatrix;
    vec3 position;
};
uniform Camera camera;

uniform sampler2D noiseTex;
uniform int sampleCount;
uniform float radius;

const int MAX_SAMPLES = 32;
vec3 kernel[MAX_SAMPLES] = vec3[]
(
   vec3(0.2150, 0.2006, 0.1975   ),
   vec3(0.3214, -0.2144, 0.5133  ),
   vec3(- 0.5527, -0.1417, 0.5493),
   vec3(- 0.4857, 0.2709, 0.2328 ),
   vec3(0.0322, 0.0530, 0.0635   ),
   vec3(-0.0049, 0.0029, 0.0016  ),
   vec3(0.0193, -0.0850, 0.0371  ),
   vec3(0.0273, -0.0143, 0.0257  ),
   vec3(- 0.0246, 0.0094, 0.0103 ),
   vec3(0.0086, -0.0321, 0.0190  ),
   vec3(- 0.0808, 0.0783, 0.0451 ),
   vec3(0.0209, -0.0224, 0.0854  ),
   vec3(- 0.0036, 0.0002, 0.0058 ),
   vec3(- 0.0142, 0.0322, 0.0247 ),
   vec3(0.0314, -0.0193, 0.0291  ),
   vec3(- 0.0002, 0.0001, 0.0002 ),
   vec3(0.1338, 0.0206, 0.1988   ),
   vec3(- 0.1816, -0.0292, 0.0617),
   vec3(- 0.0882, -0.1226, 0.1161),
   vec3(- 0.0368, 0.0016, 0.1073 ),
   vec3(- 0.0065, -0.1033, 0.2065),
   vec3(0.0140, 0.2051, 0.2840   ),
   vec3(0.0587, 0.0604, 0.1500   ),
   vec3(0.1547, -0.1773, 0.0162  ),
   vec3(0.1020, -0.0572, 0.0582  ),
   vec3(- 0.0274, -0.0251, 0.0336),
   vec3(0.0167, -0.0195, 0.0104  ),
   vec3(- 0.0900, -0.4750, 0.2303),
   vec3(0.2311, -0.1284, 0.0191  ),
   vec3(- 0.0383, -0.0705, 0.0705),
   vec3(0.1737, 0.1539, 0.0114   ),
   vec3(0.0623, 0.1574, 0.0438   )
);

float random(vec2 co)
{
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

mat3 computeTBN(vec3 normal)
{
    vec2 r = vec2(random(TexCoord.xy), random(TexCoord.yx));
    vec3 randomVec = normalize(vec3(2.0 * r - 1.0, 0.0));

    vec3 tangent = cross(randomVec, normal);
    vec3 bitangent = cross(normal, tangent);
    return mat3(tangent, bitangent, normal);
}

void main()
{
    FragmentInfo fragment = getFragmentInfo(TexCoord, albedoTex, normalTex, materialTex, depthTex, camera.invViewProjMatrix);
    mat3 TBN = computeTBN(fragment.normal);
    
    const float sampleDepth = 1.0 / fragment.depth;
    int samples = min(sampleCount, MAX_SAMPLES);
    float totalOcclusion = 0.0;
    for (int i = 0; i < samples; i++)
    {
        vec3 kernelWorldSpace = TBN * kernel[i];
        vec3 sampleVec = fragment.position + kernelWorldSpace * radius;

        vec4 frag = worldToFragSpace(sampleVec, camera.viewProjMatrix);
        float currentDepth = 1.0 / texture(depthTex, frag.xy).r;

        vec3 currentNormal = normalize(2.0 * texture(normalTex, frag.xy).rgb - 1.0);
        float Nn = 0.5 * dot(fragment.normal, currentNormal) + 0.5;

        float depthDiff = abs(sampleDepth - currentDepth);
        float bias = pow(Nn, 300.0) + 0.01 * sampleDepth;
        float rangeCheck = smoothstep(0.0, 1.0, radius / depthDiff);
        float offscreenFading = 1.0 - 4.0 * dot(TexCoord - 0.5, TexCoord - 0.5);
        offscreenFading *= offscreenFading;

        float occlusion = (sampleDepth > currentDepth + bias ? 1.0 : 0.0) * rangeCheck * offscreenFading;

        totalOcclusion += occlusion;
    }
    totalOcclusion = totalOcclusion / float(samples);
    
    OutColor = vec4(totalOcclusion, 0.0, 0.0, 1.0);
}