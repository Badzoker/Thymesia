#include "Engine_Shader_Defines.hlsli"

float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4x4		g_LightViewMatrix, g_LightProjMatrix;
float4x4		g_ViewMatrixInv, g_ProjMatrixInv;

float4          g_vLightDir, g_vLightPos, g_fRange;

float4          g_vLightDiffuse, g_vLightAmbient, g_vLightSpecular;

float4			g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
float4			g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

Texture2D		g_Texture;
Texture2D		g_DiffuseTexture;
Texture2D		g_SpecularTexture;
Texture2D		g_NormalTexture;
Texture2D		g_ShadeTexture;
Texture2D		g_DepthTexture;
Texture2D		g_ShadowTexture;

Texture2D		g_FinalTexture;

Texture2D		g_HighLightTexture;
Texture2D		g_HighLightXTexture;
Texture2D       g_HighLightYTexture;

Texture2D       g_DistortionTexture;
Texture2D       g_MotionBlurTexture;
Texture2D       g_GlowBeginTexture;
Texture2D       g_GlowXTexture;
Texture2D       g_GlowYTexture;

float4			g_vCamPosition;

float           g_fViewPortWidth, g_fViewPortHeight;

struct VS_IN
{
	float3			vPosition : POSITION;
	float2			vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
	float4			vPosition : SV_POSITION;
	float2			vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexcoord = In.vTexcoord;

	return Out;
}


struct PS_IN
{
	float4			vPosition : SV_POSITION;
	float2			vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
	float4			vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);	

	return Out;
}


struct PS_OUT_LIGHT
{
	float4			vShade : SV_TARGET0;
    float4			vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)0;

	vector		vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	vector		vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

    vector		vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    float		fViewZ     = vDepthDesc.y; 
	
    vector      vShade = saturate(max(dot(normalize(g_vLightDir * -1.f), normalize(vNormal)), 0.f) + (g_vLightAmbient * g_vMtrlAmbient));
    Out.vShade = g_vLightDiffuse * vShade;
	
    vector      vWorldPos;
	
   /* 투영공간상의 위치 */
   /* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 * 1/w */
    vWorldPos.x = In.vTexcoord.x *  2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;
	
	
	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬  */
    vWorldPos = vWorldPos.xyzw * fViewZ;
	
	/* 로컬위치 * 월드행렬 * 뷰행렬 */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	
	/* 월드위치 */
	/* 로컬위치 * 월드행렬  */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
	
    vector vReflect = reflect(normalize(g_vLightDir), normalize(vNormal));
    vector vLook = vWorldPos - g_vCamPosition; // 캠에서 해당 물체를 바라보는 look 방향을 구한거 .
	
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

	/* 투영공간상의 위치 */
	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 * 1/w */
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;

	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬  */
    vWorldPos = vWorldPos.xyzw * fViewZ;

	/* 로컬위치 * 월드행렬 * 뷰행렬 */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 월드위치 */
	/* 로컬위치 * 월드행렬  */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

    vector vLightDir = vWorldPos - g_vLightPos;
    float fLength = length(vLightDir);

    float fAtt = saturate((g_fRange - fLength) / g_fRange);


    vector vShade = saturate(max(dot(normalize(vLightDir * -1.f), normalize(vNormal)), 0.f) + (g_vLightAmbient * g_vMtrlAmbient));

    Out.vShade = g_vLightDiffuse * vShade * fAtt;

	// vector		vWorldPos = g_WorldTexture.Sample(PointSampler, In.vTexcoord);


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
    
    vector vMotionBlurBegin = g_MotionBlurTexture.Sample(LinearSampler, In.vTexcoord);
    
    Out.vColor = vFinal + vHighLight + vGlow + vMotionBlurBegin;
    
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
    
    vector vHighLight = g_HighLightTexture.Sample(LinearSampler, In.vTexcoord); //원래 선환이가 기본 텍스쳐 + 비빈거 합치는거 여기에 다 넣어버리기->g_HighLight Y Texture 에 다들어감

    Out.vColor += vHighLight;
    
    return Out;
}

PS_OUT PS_MAIN_MOTIONBLUR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    vector vMotionBlur = g_MotionBlurTexture.Sample(LinearSampler, In.vTexcoord); //속도 샘플링?
    
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
	
	/* 투영공간상의 위치 */
	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 * 1/w */
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;

	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬  */
    vWorldPos = vWorldPos.xyzw * fViewZ;

	/* 로컬위치 * 월드행렬 * 뷰행렬 */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 월드위치 */
	/* 로컬위치 * 월드행렬  */
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
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL();
	}

    pass Point //2
    {
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_SKip_Z, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL();
	}


    pass Final //3 원래 Deferred
    {
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_SKip_Z, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_FINAL();
	}

    pass HighLightX //4 원래 Blur X
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_HIGHLIGHT_X();
    }

    pass HighLightY //5 원래 Blur Y 여기에 그냥 예전 Blur_Final을 해버림
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_SKip_Z, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_HIGHLIGHT_Y();
    }

    pass Deferred //6 원래 Final
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
}
