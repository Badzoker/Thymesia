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
};

float g_fTime = 0.0167f; //1.f / 60.f //이거 안됨 상수버퍼로 던져야지 가능할듯 아직 안해봄

cbuffer GlobalCamera : register(b0)
{
    float4 g_vCamPos;
}

StructuredBuffer<Point_Particle> g_tInput_Compute : register(t0);
RWStructuredBuffer<Point_Particle> g_tOutput_Compute : register(u0);

[numthreads(256, 1, 1)]
void CSMain_Particle_Reset(int3 dispatchThreadID : SV_DispatchThreadID)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    g_tOutput_Compute[dispatchThreadID.x].vSpeed = tInput.vSpeed * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vRight = tInput.vRight * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vUp = tInput.vUp * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vLook = tInput.vLook * 1.f;
    //g_tOutput_Compute[dispatchThreadID.x].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vTranslation.x = tInput.vTranslation.x * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vTranslation.z = tInput.vTranslation.z * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vTranslation.y = tInput.vTranslation.y * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vTranslation.w = 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vLifeTime.y = 0.f;

}

[numthreads(128, 1, 1)]
void CSMain_Particle_Drop(int3 dispatchThreadID : SV_DispatchThreadID)
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

[numthreads(128, 1, 1)]
void CSMain_Particle_Explosion(int3 dispatchThreadID : SV_DispatchThreadID)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    g_tOutput_Compute[dispatchThreadID.x].vSpeed = tInput.vSpeed * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vLifeTime.y += 0.0167f;
    float fScale = tInput.vScale.x - tInput.vScale.x * (g_tOutput_Compute[dispatchThreadID.x].vLifeTime.y / g_tOutput_Compute[dispatchThreadID.x].vLifeTime.x);
    fScale = (abs(fScale - 0.5f) * -2.f) + 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vScale.x = fScale;
    
    float3 vDir = float3(normalize(tInput.vTranslation.xyz - tInput.vPivot));
    vDir = vDir * tInput.vSpeed * 0.0167f;
    g_tOutput_Compute[dispatchThreadID.x].vTranslation.xyz -= vDir;
    g_tOutput_Compute[dispatchThreadID.x].vTranslation.w = 1.f;
    
    g_tOutput_Compute[dispatchThreadID.x].vRight = float4(normalize(vDir), 0.f) * g_tOutput_Compute[dispatchThreadID.x].vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    g_tOutput_Compute[dispatchThreadID.x].vUp = vUp * tInput.vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    g_tOutput_Compute[dispatchThreadID.x].vLook = vLook * tInput.vScale.z;

    GroupMemoryBarrierWithGroupSync();
}

[numthreads(128, 1, 1)]
void CSMain_Particle_Spark(int3 dispatchThreadID : SV_DispatchThreadID)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    g_tOutput_Compute[dispatchThreadID.x].vSpeed.xz = tInput.vSpeed.xz * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vSpeed.y -= tInput.vSpeed.y * 0.0167f;
    g_tOutput_Compute[dispatchThreadID.x].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vLifeTime.y += 0.0167f;
    float fScale = tInput.vScale.x - tInput.vScale.x * (g_tOutput_Compute[dispatchThreadID.x].vLifeTime.y / g_tOutput_Compute[dispatchThreadID.x].vLifeTime.x);
    fScale = (abs(fScale - 0.5f) * -2.f) + 1.01f;
    
    if (fScale < 0.1f)
        fScale = 0.001f;
    
    g_tOutput_Compute[dispatchThreadID.x].vScale.x = fScale;
    
    float3 vDir = float3(normalize(tInput.vTranslation.xyz - tInput.vPivot));
    vDir = vDir * g_tOutput_Compute[dispatchThreadID.x].vSpeed * 0.0167f;
    g_tOutput_Compute[dispatchThreadID.x].vTranslation.xyz -= vDir;
    g_tOutput_Compute[dispatchThreadID.x].vTranslation.w = 1.f;
    
    g_tOutput_Compute[dispatchThreadID.x].vRight = float4(normalize(vDir), 0.f) * g_tOutput_Compute[dispatchThreadID.x].vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    g_tOutput_Compute[dispatchThreadID.x].vUp = vUp * tInput.vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    g_tOutput_Compute[dispatchThreadID.x].vLook = vLook * tInput.vScale.z;

    GroupMemoryBarrierWithGroupSync();
}

[numthreads(32, 1, 1)]
void CSMain_Particle_Holding(int3 dispatchThreadID : SV_DispatchThreadID)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    g_tOutput_Compute[dispatchThreadID.x].vSpeed.xz = tInput.vSpeed.xz * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vSpeed.y -= tInput.vSpeed.y * 0.0167f;
    g_tOutput_Compute[dispatchThreadID.x].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vLifeTime.y += 0.0167f;
    
    float3 vDir = float3(normalize(tInput.vTranslation.xyz - tInput.vPivot));
    
    g_tOutput_Compute[dispatchThreadID.x].vRight = float4(normalize(vDir), 0.f) * tInput.vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    g_tOutput_Compute[dispatchThreadID.x].vUp = vUp * tInput.vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    g_tOutput_Compute[dispatchThreadID.x].vLook = vLook * tInput.vScale.z;
    float fSpeed = length(g_tOutput_Compute[dispatchThreadID.x].vSpeed);
    
    if (fSpeed > length(0.0167f * tInput.vSpeed))
    {
        g_tOutput_Compute[dispatchThreadID.x].vSpeed -= 0.0167f * tInput.vSpeed;
        vDir = vDir * g_tOutput_Compute[dispatchThreadID.x].vSpeed * 0.0167f;
        
        g_tOutput_Compute[dispatchThreadID.x].vTranslation.xyz -= vDir;
        g_tOutput_Compute[dispatchThreadID.x].vTranslation.w = 1.f;
    }

    GroupMemoryBarrierWithGroupSync();
}

[numthreads(4, 1, 1)]
void CSMain_Particle_Blood(int3 dispatchThreadID : SV_DispatchThreadID)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    float fSpeed = length(g_tOutput_Compute[dispatchThreadID.x].vSpeed);
    g_tOutput_Compute[dispatchThreadID.x].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vLifeTime.y += 0.0167f * 2.f;
    
    float3 vDir = float3(normalize(tInput.vTranslation.xyz - tInput.vPivot));
    
    g_tOutput_Compute[dispatchThreadID.x].vRight = float4(normalize(vDir), 0.f) * tInput.vScale.x;
    float4 vUp = normalize(float4(cross(vDir, float3(0.f, 0.f, 1.f)), 0.f));
    g_tOutput_Compute[dispatchThreadID.x].vUp = vUp * tInput.vScale.y;
    float4 vLook = normalize(float4(cross(vUp.xyz, vDir), 0.f));
    g_tOutput_Compute[dispatchThreadID.x].vLook = vLook * tInput.vScale.z;
    
    if (fSpeed > length(0.0167f * tInput.vSpeed * 2.f))
    {
        g_tOutput_Compute[dispatchThreadID.x].vSpeed -= 0.0167f * 2.f * tInput.vSpeed;
        vDir = vDir * g_tOutput_Compute[dispatchThreadID.x].vSpeed * 0.0167f;
        
        g_tOutput_Compute[dispatchThreadID.x].vTranslation.xyz -= vDir;
        g_tOutput_Compute[dispatchThreadID.x].vTranslation.w = 1.f;
    }

    GroupMemoryBarrierWithGroupSync();
}

[numthreads(128, 1, 1)]
void CSMain_Particle_Dust(int3 dispatchThreadID : SV_DispatchThreadID)
{
    Point_Particle tInput = g_tInput_Compute[dispatchThreadID.x];
    g_tOutput_Compute[dispatchThreadID.x].vSpeed.xz = tInput.vSpeed.xz * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vSpeed.y -= tInput.vSpeed.y * 0.0167f;
    g_tOutput_Compute[dispatchThreadID.x].vLifeTime.x = tInput.vLifeTime.x * 1.f;
    g_tOutput_Compute[dispatchThreadID.x].vLifeTime.y += 0.0167f;
    float fScale = tInput.vScale.x * (g_tOutput_Compute[dispatchThreadID.x].vLifeTime.y / g_tOutput_Compute[dispatchThreadID.x].vLifeTime.x);
    
    if (fScale < 0.1f)
        fScale = 0.001f;
    
    g_tOutput_Compute[dispatchThreadID.x].vScale.x = fScale;
    
    float3 vDir = float3(normalize(tInput.vPivot - tInput.vTranslation.xyz));
    vDir = vDir * g_tOutput_Compute[dispatchThreadID.x].vSpeed * 0.0167f;
    g_tOutput_Compute[dispatchThreadID.x].vTranslation.xyz -= vDir;
    g_tOutput_Compute[dispatchThreadID.x].vTranslation.w = 1.f;
    
    g_tOutput_Compute[dispatchThreadID.x].vLook = vector(normalize(g_vCamPos.xyz - g_tOutput_Compute[dispatchThreadID.x].vTranslation.xyz) * tInput.vScale.z, 0.f);
    g_tOutput_Compute[dispatchThreadID.x].vRight = normalize(vector(cross(float3(0.f, 1.f, 0.f), g_tOutput_Compute[dispatchThreadID.x].vLook.xyz), 0.f)) * g_tOutput_Compute[dispatchThreadID.x].vScale.x;
    g_tOutput_Compute[dispatchThreadID.x].vUp = normalize(vector(cross(g_tOutput_Compute[dispatchThreadID.x].vLook.xyz, g_tOutput_Compute[dispatchThreadID.x].vRight.xyz), 0.f)) * tInput.vScale.y;

    GroupMemoryBarrierWithGroupSync();
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
}