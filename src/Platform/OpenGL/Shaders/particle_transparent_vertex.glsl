layout(location = 0) in vec4 position;

out vec2 TexCoord;
out vec3 WorldPosition;
out float TransparencyFading;
out float LinearDepth;

struct Particle
{
    vec4 position_timeAlive;
    vec4 velocity_size;
    vec4 angularParams_spawnDistance;
};

layout(std430, binding = 0) buffer ParticleData
{
    Particle particleData[];
};

uniform mat4 projMatrix;
uniform float aspectRatio;
uniform float fading;
uniform float lifetime;
uniform mat4 transform;

void main()
{
    Particle particle = particleData[gl_InstanceID];
    vec3 particlePosition = particle.position_timeAlive.xyz;
    float particleTimeAlive = particle.position_timeAlive.w;
    float particleSize = particle.velocity_size.w;

    vec4 projectedPosition = projMatrix * transform * vec4(particlePosition, 1.0);
    projectedPosition.xy += particleSize * vec2(position.x, aspectRatio * position.y);

    gl_Position = projectedPosition;
    WorldPosition = particlePosition;
    TexCoord = position.xy * 0.5 + vec2(0.5);
    LinearDepth = gl_Position.w / gl_Position.z;
    TransparencyFading = pow(1.0 - particleTimeAlive / lifetime, fading);
}