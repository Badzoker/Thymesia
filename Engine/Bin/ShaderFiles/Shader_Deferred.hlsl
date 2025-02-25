#include "Engine_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4x4 g_LightViewMatrix, g_LightProjMatrix;
float4x4 g_ViewMatrixInv, g_ProjMatrixInv;

float4 g_vLightDir, g_vLightPos;
float g_fRange, g_fSpot;

float4 g_vLightDiffuse, g_vLightAmbient, g_vLightSpecular;

float4 g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
float4 g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

Texture2D g_Texture;
Texture2D g_DiffuseTexture;
Texture2D g_SpecularTexture;
Texture2D g_NormalTexture;
Texture2D g_ShadeTexture;
Texture2D g_DepthTexture;
Texture2D g_ShadowTexture;
texture2D g_FogTexture;
Texture2D g_OccusionTexture;
Texture2D g_LightShaftXTexture;
Texture2D g_LightShaftYTexture;

Texture2D g_FinalTexture;

Texture2D g_HighLightTexture;
Texture2D g_HighLightXTexture;
Texture2D g_HighLightYTexture;

Texture2D g_DistortionTexture;
Texture2D g_MotionBlurTexture;
Texture2D g_GlowBeginTexture;
Texture2D g_GlowXTexture;
Texture2D g_GlowYTexture;

float4 g_vCamPosition;

float g_fViewPortWidth, g_fViewPortHeight;

float g_FogRange;

float4 g_LightShaftValue;

float2 g_ScreenLightPos;


//g_LightShaftValue . x == Density
//y == Decay
//z == Weight
//w == Exposure

float4 psLightShaft(float2 texcoord)
{
    int NUM_SAMPLES = 128;
    
    float2 DeltaTexCoord = (texcoord.xy - g_ScreenLightPos.xy);
    
    DeltaTexCoord *= 1.f / (float) NUM_SAMPLES * g_LightShaftValue.x;
    
    float4 vColor = g_OccusionTexture.Sample(LinearSampler, texcoord);
    
    float IlluminationDecay = 1.f;
    
    for (int i = 0; i < NUM_SAMPLES; ++i)
    {
        texcoord -= DeltaTexCoord;
        
        float4 Sample2 = g_OccusionTexture.Sample(LinearSampler, texcoord);
        
        Sample2 *= IlluminationDecay * g_LightShaftValue.z;
        
        vColor += Sample2;
        
        IlluminationDecay *= g_LightShaftValue.y;
    }
    
    return saturate(vColor * g_LightShaftValue.w);
}

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;

    return Out;
}


struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    return Out;
}


struct PS_OUT_LIGHT
{
    float4 vShade : SV_TARGET0;
    float4 vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(PS_IN In)
{
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;

    vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
    vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.y;
	
    vector vShade = saturate(max(dot(normalize(g_vLightDir * -1.f), normalize(vNormal)), 0.f) + (g_vLightAmbient * g_vMtrlAmbient));
    Out.vShade = g_vLightDiffuse * vShade;
	
    vector vWorldPos;
	
   /* ������������ ��ġ */
   /* ������ġ * ������� * ����� * ������� * 1/w */
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;
	
	
	/* ������ġ * ������� * ����� * �������  */
    vWorldPos = vWorldPos.xyzw * fViewZ;
	
	/* ������ġ * ������� * ����� */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	
	/* ������ġ */
	/* ������ġ * �������  */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
	
    vector vReflect = reflect(normalize(g_vLightDir), normalize(vNormal));
    vector vLook = vWorldPos - g_vCamPosition; // ķ���� �ش� ��ü�� �ٶ󺸴� look ������ ���Ѱ� .
	
    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vReflect * -1.f), normalize(vLook)), 0.f), 90.f);

    return Out;

}

PS_OUT_LIGHT PS_MAIN_POINT(PS_IN In)
{
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;

    vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
    vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.y;

    vector vWorldPos;

	/* ������������ ��ġ */
	/* ������ġ * ������� * ����� * ������� * 1/w */
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;

	/* ������ġ * ������� * ����� * �������  */
    vWorldPos = vWorldPos.xyzw * fViewZ;

	/* ������ġ * ������� * ����� */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* ������ġ */
	/* ������ġ * �������  */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

    vector vLightDir = vWorldPos - g_vLightPos;
    float fLength = length(vLightDir);

    float fAtt = saturate((g_fRange - fLength) / g_fRange);


    vector vShade = saturate(max(dot(normalize(vLightDir * -1.f), normalize(vNormal)), 0.f) + (g_vLightAmbient * g_vMtrlAmbient));

    Out.vShade = g_vLightDiffuse * vShade * fAtt;

    vector vReflect = reflect(normalize(vLightDir), normalize(vNormal));
    vector vLook = vWorldPos - g_vCamPosition;

    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vReflect * -1.f), normalize(vLook)), 0.f), 50.f) * fAtt;
    
    return Out;
}


PS_OUT_LIGHT PS_MAIN_SPOT(PS_IN In)
{
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;

    vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
    vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.y;

    vector vWorldPos;

	/* ������������ ��ġ */
	/* ������ġ * ������� * ����� * ������� * 1/w */
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;

	/* ������ġ * ������� * ����� * �������  */
    vWorldPos = vWorldPos.xyzw * fViewZ;

	/* ������ġ * ������� * ����� */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
    
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    vector vLightDir = vWorldPos - g_vLightPos;
    
    float fLength = length(vLightDir);

    float fAtt = saturate((g_fRange - fLength) / g_fRange);

    vector vShade;
    
    vShade = saturate(max(dot(normalize(vLightDir * -1.f), normalize(vNormal)), 0.f) + (g_vLightAmbient * g_vMtrlAmbient));
    
    float fAngle = pow(max(dot(normalize(-vLightDir), normalize(-g_vLightDir)), 0.f), g_fSpot);
    
    float Att = fAngle / dot(fAtt, float3(1.f, 0.f, g_fRange * g_fRange));
    
    Out.vShade = g_vLightDiffuse * vShade * fAngle * Att;
   
    vector vReflect = reflect(normalize(vLightDir), normalize(vNormal));
    vector vLook = vWorldPos - g_vCamPosition;

    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vReflect * -1.f), normalize(vLook)), 0.f), 50.f) * fAtt;
    
    return Out;
}

PS_OUT PS_MAIN_FINAL(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    vector vDistortion = g_DistortionTexture.Sample(LinearSampler, In.vTexcoord);
    
    float fWeight = vDistortion.r * 1.2f;

    vector vFinal = g_FinalTexture.Sample(LinearSampler, In.vTexcoord + fWeight);
    
    vector vHighLight = g_HighLightYTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector vGlow = g_GlowYTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector vDepth = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector vGodRay = g_LightShaftYTexture.Sample(LinearSampler, In.vTexcoord);
    
    float fViewZ = vDepth.y;
	
    vector vWorldPos;
	
	/* ������������ ��ġ */
	/* ������ġ * ������� * ����� * ������� * 1/w */
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepth.x;
    vWorldPos.w = 1.f;
   
	/* ������ġ * ������� * ����� * �������  */
    vWorldPos = vWorldPos.xyzw * fViewZ;
    
    float FogFactor = 1.f - saturate((1.f / 2.71828f) * pow((vWorldPos.z * g_FogRange), 2.f));
    
    vector vMotionBlurBegin = g_MotionBlurTexture.Sample(LinearSampler, In.vTexcoord);
    
    Out.vColor = vFinal + vHighLight + vGlow + vMotionBlurBegin + vGodRay;
    
    float3 vFogGodRay = vGodRay.xyz;
    
    vector FogColor = lerp(vector(0.5, 0.5, 0.5, 1.f), vector(1.0, 0.9, 0.7, 1.f), length(vFogGodRay));
    
    //if (vWorldPos.z <= 0.f)
    //    Out.vColor = float4(0.5f, 0.5f, 0.5f, 1.f);
    //else
    //    Out.vColor = FogFactor * Out.vColor + (1.f - FogFactor) * FogColor;
    
    return Out;
}

float g_fWeights_HighLight[13] =
{
    0.0561, 0.1353, 0.278, 0.4868, 0.7261, 0.9231, 1.f, 0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561
};

//float g_fWeights_HighLight[13] =
//{
//    0.00903f, 0.02178f, 0.04475f, 0.07842f, 0.11698f, 0.14866f, 0.16105f,
//    0.14866f, 0.11698f, 0.07842f, 0.04475f, 0.02178f, 0.00903f
//};

PS_OUT PS_MAIN_HIGHLIGHT_X(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float2 vTexcoord = 0.f;

    for (int i = -6; i < 7; i++)
    {
        
        //vTexcoord = float2(In.vTexcoord.x + (1.f / 1600.f) * i, In.vTexcoord.y);	
        vTexcoord = float2(In.vTexcoord.x + (1.f / g_fViewPortWidth) * i, In.vTexcoord.y);

        Out.vColor += g_fWeights_HighLight[i + 6] * g_HighLightTexture.Sample(LinearSampler_Clamp, vTexcoord);
    }

    Out.vColor /= 6.f;

    return Out;
}

PS_OUT PS_MAIN_HIGHLIGHT_Y(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float2 vTexcoord = 0.f;

    for (int i = -6; i < 7; i++)
    {
        //vTexcoord = float2(In.vTexcoord.x, In.vTexcoord.y + (1.f / 900.f) * i);
        vTexcoord = float2(In.vTexcoord.x, In.vTexcoord.y + (1.f / g_fViewPortHeight) * i);

        Out.vColor += g_fWeights_HighLight[i + 6] * g_HighLightXTexture.Sample(LinearSampler_Clamp, vTexcoord);
    }

    Out.vColor /= 6.f;
    
    vector vHighLight = g_HighLightTexture.Sample(LinearSampler, In.vTexcoord); //���� ��ȯ�̰� �⺻ �ؽ��� + ���� ��ġ�°� ���⿡ �� �־������->g_HighLight Y Texture �� �ٵ�

    Out.vColor += vHighLight;
    
    return Out;
}

PS_OUT PS_MAIN_MOTIONBLUR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    vector vMotionBlur = g_MotionBlurTexture.Sample(LinearSampler, In.vTexcoord); //�ӵ� ���ø�?
    
    float2 vTexcoord = 0.f;
    
    float Amount_X = (vMotionBlur.r / g_fViewPortWidth);
    float Amount_Y = (vMotionBlur.g / g_fViewPortHeight);
    
    vector vFinal_Color_MotionBlur;
    
    if (vMotionBlur.r > 0.5f || vMotionBlur.g > 0.5f)
    {
        for (int i = -6; i < 7; i++)
        {
            vTexcoord = float2(In.vTexcoord.x + (Amount_X * 8.f) * i, In.vTexcoord.y + (Amount_Y * 8.f) * i);
            Out.vColor += g_fWeights_HighLight[i + 6] * g_FinalTexture.Sample(LinearSampler, vTexcoord);
        }
    }
    
    Out.vColor /= 13.f;
    
    return Out;
}

PS_OUT PS_MAIN_DEFERRED(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    if (0.f == vDiffuse.a)
        discard;

    vector vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexcoord);
    vector vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexcoord);

    Out.vColor = vDiffuse * vShade + vSpecular;
	
    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.y;
	
    vector vWorldPos;
	
	/* ������������ ��ġ */
	/* ������ġ * ������� * ����� * ������� * 1/w */
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;
   
	/* ������ġ * ������� * ����� * �������  */
    vWorldPos = vWorldPos.xyzw * fViewZ;

	/* ������ġ * ������� * ����� */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	/* ������ġ */

	/* ������ġ * �������  */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

    vWorldPos = mul(vWorldPos, g_LightViewMatrix);
    vWorldPos = mul(vWorldPos, g_LightProjMatrix);
    
    float2 vTexcoord;

    vTexcoord.x = (vWorldPos.x / vWorldPos.w) * 0.5f + 0.5f;
    vTexcoord.y = (vWorldPos.y / vWorldPos.w) * -0.5f + 0.5f;

    vector vShadowDepthDesc = g_ShadowTexture.Sample(LinearSampler, vTexcoord);
    
    if (vShadowDepthDesc.y + 0.15f <= vWorldPos.w)
        Out.vColor = Out.vColor * 0.7f;
    
    return Out;
}

float g_fWeights_Glow[11] = { 0.019, 0.081, 0.136, 0.370, 0.604, 1.f, 0.604, 0.370, 0.136, 0.081, 0.019 };

PS_OUT PS_MAIN_GLOW_X(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float2 vTexcoord = 0.f;

    for (int i = -5; i < 6; i++)
    {
        
        //vTexcoord = float2(In.vTexcoord.x + (1.f / 1600.f) * i, In.vTexcoord.y);	
        vTexcoord = float2(In.vTexcoord.x + (1.f / g_fViewPortWidth) * i, In.vTexcoord.y);

        Out.vColor += g_fWeights_Glow[i + 5] * g_GlowBeginTexture.Sample(LinearSampler_Clamp, vTexcoord);
    }

    Out.vColor /= 1.5f;

    return Out;
}

PS_OUT PS_MAIN_GLOW_Y(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float2 vTexcoord = 0.f;

    for (int i = -5; i < 6; i++)
    {
        
        //vTexcoord = float2(In.vTexcoord.x + (1.f / 1600.f) * i, In.vTexcoord.y);	
        vTexcoord = float2(In.vTexcoord.x, In.vTexcoord.y + (1.f / g_fViewPortHeight) * i);

        Out.vColor += g_fWeights_Glow[i + 5] * g_GlowXTexture.Sample(LinearSampler_Clamp, vTexcoord);
    }

    Out.vColor /= 1.5f;

    return Out;
}

float g_fWeights_GodRay[31] =
{
    0.0119, 0.0210, 0.0350, 0.0568, 0.0880, 0.1315, 0.1882, 0.2598, 0.3437, 0.4369, 0.5328, 0.6262, 0.7060, 0.7648, 0.7960, 1.f,
   0.7960, 0.7648, 0.7060, 0.6262, 0.5328, 0.4369, 0.3437, 0.2598, 0.1882, 0.1315, 0.0880, 0.0568, 0.0350, 0.0210, 0.0119
};

PS_OUT PS_MAIN_GODRAY_X(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float2 vTexcoord = 0.f;
    
    float4 LightShaft = psLightShaft(In.vTexcoord);
    
    for (int i = -15; i < 16; i++)
    {
        vTexcoord = float2(In.vTexcoord.x + (1.f / g_fViewPortWidth) * i, In.vTexcoord.y);

        Out.vColor += g_fWeights_GodRay[i + 15] * LightShaft;
    }
    
    Out.vColor /= 15.f;

    return Out;
}

PS_OUT PS_MAIN_GODRAY_Y(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float2 vTexcoord = 0.f;

    for (int i = -15; i < 16; i++)
    {
        vTexcoord = float2(In.vTexcoord.x, In.vTexcoord.y + (1.f / g_fViewPortHeight) * i);

        Out.vColor += g_fWeights_GodRay[i + 15] * g_LightShaftXTexture.Sample(LinearSampler_Clamp, vTexcoord);
    }
    
    Out.vColor /= 15.f;
    
    return Out;
}

technique11 DefaultTechnique
{
    pass Debug //0 
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
    }

    pass Directional //1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL();
    }

    pass Point //2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POINT();
    }


    pass Final //3 ���� Deferred
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FINAL();
    }

    pass HighLightX //4 ���� Blur X
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_HIGHLIGHT_X();
    }

    pass HighLightY //5 ���� Blur Y ���⿡ �׳� ���� Blur_Final�� �ع���
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_HIGHLIGHT_Y();
    }

    pass Deferred //6 ���� Final
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DEFERRED();
    }

    pass GlowX //7
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GLOW_X();
    }

    pass GlowY //8
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GLOW_Y();
    }

    pass MotionBlur //8
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_MOTIONBLUR();
    }

    pass Spot //10
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SPOT();
    }

    pass GodRayX //11
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GODRAY_X();
    }


    pass GodRayY //12
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GODRAY_Y();
    }
}
