#include "Engine_Shader_Defines.hlsli"

struct Point_Particle
{
    float4 vRight;
    float4 vUp;
    float4 vLook;
    float4 vTranslation;
    float2 vLifeTime;
    float3 vSpeed;
    float3 vPivot;
    float3 vScale;
    float fDelayTime;
};

float g_fTime = 0.0167f; //1.f / 60.f //이거 안됨 상수버퍼로 던져야지 가능할듯 아직 안해봄

StructuredBuffer<Point_Particle> g_tInput_Compute : register(t0);
RWStructuredBuffer<Point_Particle> g_tOutput_Compute : register(u0);

groupshared Point_Particle sharedParticles[256];

[numthreads(256, 1, 1)]
void CSMain_Particle_Reset(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].vSpeed = tInput.vSpeed * 1.f;
    sharedParticles[groupIndex].vRight = tInput.vRight * 1.f;
    sharedParticles[groupIndex].vUp = tInput.vUp * 1.f;
    sharedParticles[groupIndex].vLook = tInput.vLook * 1.f;
    sharedParticles[groupIndex].vTranslation.x = tInput.vTranslation.x * 1.f;
    sharedParticles[groupIndex].vTranslation.z = tInput.vTranslation.z * 1.f;
    sharedParticles[groupIndex].vTranslation.y = tInput.vTranslation.y * 1.f;
    sharedParticles[groupIndex].vTranslation.w = 1.f;
    sharedParticles[groupIndex].vLifeTime.y = 0.f;
    sharedParticles[groupIndex].fDelayTime = 0.f;
    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Drop(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    g_tOutput_Compute[dispatchThreadID.x].vSpeed = tInput.vSpeed * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vRight = tInput.vRight * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vUp = tInput.vUp * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vLook = tInput.vLook * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vTranslation.x = tInput.vTranslation.x * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vTranslation.z = tInput.vTranslation.z * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vTranslation.y -= tInput.vSpeed.y * 0.0167f;
    g_tOutput_Compute[dispatchThreadID.x].vTranslation.w = 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vLifeTime.y += 0.0167f;
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Explosion(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    sharedParticles[groupIndex].vSpeed = tInput.vSpeed * 1.f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
    float fScale = tInput.vScale.x - tInput.vScale.x * (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x);
    fScale = (abs(fScale - 0.5f) * -2.f) + 1.f;
    sharedParticles[groupIndex].vScale.x = fScale;
    
    float3 vDir = float3(normalize(tInput.vTranslation.xyz - tInput.vPivot));
    vDir = vDir * tInput.vSpeed * 0.0167f;
    sharedParticles[groupIndex].vTranslation.xyz -= vDir;
    sharedParticles[groupIndex].vTranslation.w = 1.f;
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * sharedParticles[groupIndex].vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * tInput.vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * tInput.vScale.z;

    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Spark(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
    sharedParticles[groupIndex].vSpeed.xz = tInput.vSpeed.xz * 1.f;
    sharedParticles[groupIndex].vSpeed.y -= tInput.vSpeed.y * 0.0167f * (1.f - (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x));
    float fScale = tInput.vScale.x - tInput.vScale.x * (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x);
    fScale = (abs(fScale - 0.5f) * -2.f) + 1.01f;
    
    if (fScale < 0.1f)
        fScale = 0.001f;
    
    sharedParticles[groupIndex].vScale.x = fScale;
    
    float3 vDir = float3(normalize(tInput.vTranslation.xyz - tInput.vPivot));
    vDir = vDir * sharedParticles[groupIndex].vSpeed * 0.0167f;
    sharedParticles[groupIndex].vTranslation.xyz -= vDir;
    sharedParticles[groupIndex].vTranslation.w = 1.f;
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * fScale;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * fScale;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * fScale;

    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Holding(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].vSpeed.xz = tInput.vSpeed.xz * 1.f;
    sharedParticles[groupIndex].vSpeed.y -= tInput.vSpeed.y * 0.0167f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
    
    float3 vDir = float3(normalize(tInput.vTranslation.xyz - tInput.vPivot));
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * tInput.vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * tInput.vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * tInput.vScale.z;
    float fSpeed = length(sharedParticles[groupIndex].vSpeed);
    
    if (fSpeed > length(0.0167f * tInput.vSpeed))
    {
        sharedParticles[groupIndex].vSpeed -= 0.0167f * tInput.vSpeed;
        vDir = vDir * sharedParticles[groupIndex].vSpeed * 0.0167f;
        
        sharedParticles[groupIndex].vTranslation.xyz -= vDir;
        sharedParticles[groupIndex].vTranslation.w = 1.f;
    }

    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Blood(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    float fSpeed = length(sharedParticles[groupIndex].vSpeed);
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    sharedParticles[groupIndex].vLifeTime.y += 0.0167f * 2.f;
    
    float3 vDir = float3(normalize(tInput.vTranslation.xyz - tInput.vPivot));
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * tInput.vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * tInput.vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * tInput.vScale.z;
    
    if (fSpeed > length(0.0167f * tInput.vSpeed * 2.f))
    {
        sharedParticles[groupIndex].vSpeed -= 0.0167f * 2.f * tInput.vSpeed;
        vDir = vDir * sharedParticles[groupIndex].vSpeed * 0.0167f;
        
        sharedParticles[groupIndex].vTranslation.xyz -= vDir;
        sharedParticles[groupIndex].vTranslation.w = 1.f;
    }

    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Dust(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].vSpeed.xz = tInput.vSpeed.xz * 1.f;
    sharedParticles[groupIndex].vSpeed.y -= tInput.vSpeed.y * 0.0167f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
    float fScale = tInput.vScale.x * (sharedParticles[groupIndex].vLifeTime.y / sharedParticles[groupIndex].vLifeTime.x);
    
    if (fScale < 0.1f)
        fScale = 0.001f;
    
    sharedParticles[groupIndex].vScale.x = fScale;
    
    float3 vDir = float3(normalize(tInput.vPivot - tInput.vTranslation.xyz));
    vDir = vDir * sharedParticles[groupIndex].vSpeed * 0.0167f;
    sharedParticles[groupIndex].vTranslation.xyz -= vDir;
    sharedParticles[groupIndex].vTranslation.w = 1.f;
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * fScale;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * fScale;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * fScale;
    
    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

[numthreads(256, 1, 1)]
void CSMain_Particle_Dust_Delay(int3 dispatchThreadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    
    sharedParticles[groupIndex] = g_tOutput_Compute[dispatchThreadID.x];
    GroupMemoryBarrierWithGroupSync();
    
    sharedParticles[groupIndex].vSpeed.xz = tInput.vSpeed.xz * 1.f;
    sharedParticles[groupIndex].vSpeed.y -= tInput.vSpeed.y * 0.0167f;
    
    
    sharedParticles[groupIndex].fDelayTime += 0.0167f;
    sharedParticles[groupIndex].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    
    float3 vDir = float3(normalize(tInput.vPivot - tInput.vTranslation.xyz));
    if (tInput.fDelayTime > sharedParticles[groupIndex].fDelayTime)
    {
        sharedParticles[groupIndex].vLifeTime.x = 0.001f;
        sharedParticles[groupIndex].vLifeTime.y = 0.f;
        sharedParticles[groupIndex].vScale = 0.f;
    }
    else
    {
        sharedParticles[groupIndex].vScale = tInput.vScale;
        sharedParticles[groupIndex].vLifeTime.y += 0.0167f;
        vDir = vDir * sharedParticles[groupIndex].vSpeed * 0.0167f;
        sharedParticles[groupIndex].vTranslation.xyz -= vDir;
        sharedParticles[groupIndex].vTranslation.w = 1.f;
    }
    
    sharedParticles[groupIndex].vRight = float4(normalize(vDir), 0.f) * tInput.vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    sharedParticles[groupIndex].vUp = vUp * tInput.vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    sharedParticles[groupIndex].vLook = vLook * tInput.vScale.z;
    
    GroupMemoryBarrierWithGroupSync();
    
    g_tOutput_Compute[dispatchThreadID.x] = sharedParticles[groupIndex];
}

technique11 DefaultTechnique
{
    pass ParticleReset //0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Reset()));
    }

    pass ParticleDrop //1
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Drop()));
    }

    pass ParticleExplosion //2
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Explosion()));
    }

    pass ParticleSpark //3
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Spark()));
    }

    pass ParticleHolding //4
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Holding()));
    }

    pass ParticleBlood //5
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Blood()));
    }

    pass ParticleDust //6
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Dust()));
    }

    pass ParticleDust_Delay //7
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Dust_Delay()));
    }
}