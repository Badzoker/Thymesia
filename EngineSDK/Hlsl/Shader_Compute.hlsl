#include "Engine_Shader_Defines.hlsli"

struct Point_Particle_Drop
{
    float4 vRight;
    float4 vUp;
    float4 vLook;
    float4 vTranslation;
    float2 vLifeTime;
    float fSpeed;
};

float g_fTime = 0.0167f; //1.f / 60.f //이거 안됨 상수버퍼로 던져야지 가능할듯 아직 안해봄

StructuredBuffer<Point_Particle_Drop> g_tInput_Drop : register(t0);
RWStructuredBuffer<Point_Particle_Drop> g_tOutput_Drop : register(u0);


[numthreads(128, 1, 1)]
void CSMain_Particle_Drop(int3 dispatchThreadID : SV_DispatchThreadID)
{
    Point_Particle_Drop pTest = g_tInput_Drop[dispatchThreadID.x];
    g_tOutput_Drop[dispatchThreadID.x].fSpeed = pTest.fSpeed * 1.f;
    g_tOutput_Drop[dispatchThreadID.x].vRight = pTest.vRight * 1.f;
    g_tOutput_Drop[dispatchThreadID.x].vUp = pTest.vUp * 1.f;
    g_tOutput_Drop[dispatchThreadID.x].vLook = pTest.vLook * 1.f;
    g_tOutput_Drop[dispatchThreadID.x].vLifeTime.x = pTest.vLifeTime.x * 1.f;
    g_tOutput_Drop[dispatchThreadID.x].vTranslation.x = pTest.vTranslation.x * 1.f;
    g_tOutput_Drop[dispatchThreadID.x].vTranslation.z = pTest.vTranslation.z * 1.f;
    g_tOutput_Drop[dispatchThreadID.x].vTranslation.y -= pTest.fSpeed * 1.f / 60.f;
    g_tOutput_Drop[dispatchThreadID.x].vLifeTime.y += 1.f / 60.f;

    if (pTest.vLifeTime.x <= g_tOutput_Drop[dispatchThreadID.x].vLifeTime.y)
    {
        g_tOutput_Drop[dispatchThreadID.x].vLifeTime.y = 0.f;
        g_tOutput_Drop[dispatchThreadID.x].vTranslation.y = pTest.vTranslation.y * 1.f;
    }
}

technique11 DefaultTechnique
{
    pass ParticleDrop //0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CSMain_Particle_Drop()));
    }
}