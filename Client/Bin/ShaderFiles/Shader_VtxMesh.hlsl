#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix, g_OldViewMatrix, g_OldWorldMatrix;
Texture2D g_DiffuseTexture;
Texture2D g_NormalTexture;
Texture2D g_NoiseTexture;

float g_Time;
float g_DissolveAmount;

vector g_vCamPosition;

matrix g_LightViewMatrix[3];
matrix g_LightProjMatrix[3];

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
	
	   
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
};

struct VS_OUT_SHADOW
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT_MotionBlur
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
	   
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
    
    float4 vDir : TEXCOORD3;
};

struct VS_OUT_OCCULUSION
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
	
	   
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition;
	
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), g_WorldMatrix));
    Out.vBinormal = vector(normalize(cross(Out.vNormal.xyz, Out.vTangent.xyz)), 0.f); // ���� ���� �߿��ϴ� �ֳ��ϸ� �����ٲ�� binormal�� - ������ �����Ǵϱ� 
	
    return Out;
}

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN In)
{
    VS_OUT_SHADOW Out = (VS_OUT_SHADOW) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    Out.vTexcoord = In.vTexcoord;
    //Out.vProjPos = Out.vPosition;

    return Out;
}

VS_OUT_MotionBlur VS_MAIN_MOTIONBLUR(VS_IN In)
{
    VS_OUT_MotionBlur Out = (VS_OUT_MotionBlur) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition;
    
    //motion blur�� ���� �߰��۾�����
    float4 vNewPos = Out.vPosition;
    float4 vOldPos = mul(vector(In.vPosition, 1.f), g_OldWorldMatrix);
    vOldPos = mul(vOldPos, g_OldViewMatrix);
    vOldPos = mul(vOldPos, g_ProjMatrix);
	
    float3 vDir = vNewPos.xyz - vOldPos.xyz;
    
    float a = dot(normalize(vDir), normalize(Out.vNormal.xyz));
    if (a < 0.f)
        Out.vPosition = vOldPos;
    else
        Out.vPosition = vNewPos;
    
    float2 vVelocity = (vNewPos.xy / vNewPos.w) - (vOldPos.xy / vOldPos.w);
    Out.vDir.x = vVelocity.x * 0.5f;
    Out.vDir.y = vVelocity.y * (-0.5f);
    Out.vDir.z = Out.vPosition.z;
    Out.vDir.w = Out.vPosition.w;
    //motion blur�� ���� �߰��۾���
    
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), g_WorldMatrix));
    Out.vBinormal = vector(normalize(cross(Out.vNormal.xyz, Out.vTangent.xyz)), 0.f);
	
    return Out;
}

VS_OUT VS_MAIN_OCCULUSION(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition;
    
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
	
		
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
};

struct PS_IN_SHADOW
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_IN_MOTIONBLUR
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
	   
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
    
    float4 vDir : TEXCOORD3;
};

struct PS_IN_OCCULUSION
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
	
	   
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};

struct PS_OUT_DISTORTION
{
    float4 vDistortion : SV_TARGET0;
};

struct PS_OUT_GLOW
{
    float4 vGlow : SV_TARGET0;
};

struct PS_OUT_MOTIONBLUR
{
    float4 vMotionBlur : SV_TARGET0;
};


PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
	
    if (vMtrlDiffuse.a < 0.1f)
        discard;
	
    float4 vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	

	/* ź��Ʈ �����̽��� �����ϴ� ����̴�. */	
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
	
	
	/* ���� �����̽����� �븻�� ��ȯ����. */
    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
    vNormal = normalize(mul(vNormal, WorldMatrix));
	

    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
    //Out.vNormal  = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);
	
    return Out;
}


PS_OUT PS_MAIN_DISSOLVE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    
    if (vMtrlDiffuse.a < 0.1f)  
        discard;
   
    /* Dissolve ���� */ 
    
    // ������ �ؽ�ó���� ���ø� (UV ������, Ÿ�� ���� �̿��� ���� ����)
    float2 noiseUV = In.vTexcoord + float2(0.0f, g_Time * 0.1);
    float noiseValue = g_NoiseTexture.Sample(LinearSampler, noiseUV).r;
    
    
    //(����) Dissolve ��踦 ������ Edge Color�� ���� 
    // ���� (noiseValue)�� gDissolveAmount ��ó�� ���� �� 
    // ��) edgeWidth ���� �������� ȿ�� 
    float g_EdgeWidth = 0.3f;
    float edgeFactor = smoothstep(g_DissolveAmount - g_EdgeWidth, g_DissolveAmount, noiseValue);
    
    //edgeFactor�� �̿��� ��� ���� ���� 
    //edgeFactor�� 1�̸� ��� ��(����), 0�̸� ��� �� (���� Ŭ��)    
    //��� = edgeFactor�� 0~1 ���̸� ������ �κ�
    //(�� �� �����ϰ� �Ϸ��� saturate, lerp�� �̿�) 
    
    float4 g_EdgeColor = { 1.f, 1.f, 0.f, 1.f };
    float edgeStrength = 1.0 - edgeFactor; // ��� �αٿ��� 1
    float4 edgeBlend = lerp(vMtrlDiffuse, g_EdgeColor, edgeStrength);
    float4 finalColor = lerp(vMtrlDiffuse, edgeBlend, edgeStrength);
    
    // Dissolve ���� 
    // noiseValue < gDissolveAmount �̸� �ȼ��� ������ ( ���� ó�� ) 
    // clip(x)�� ȣ���ϸ� x < 0 �� �� �ش� �ȼ��� ������. 
    if (noiseValue < g_DissolveAmount)
    {
           // ���� ���, noiseValue�� g_DissolveAmount ��ó�� ���� edge ȿ���� �����ϰ�,
            // �׺��� ���� ������ �����ϰ� ����.
        float alphaFactor = saturate((noiseValue - (g_DissolveAmount - g_EdgeWidth)) / g_EdgeWidth);
        finalColor.a *= alphaFactor;
        
        

         // �ʿ信 ����, alpha�� �ʹ� ������ ������ ����� ���� ����
        if (finalColor.a < 0.01f) 
            clip(-1);
       
    }
    
    // (����) Dissolve ��踦 ������ Edge Color�� ���� 
    // ���� (noiseValue)�� gDissolveAmount ��ó�� ���� �� 
    // ��) edgeWidth ���� �������� ȿ�� 
    //float edgeFactor = smoothstep(g_DissolveAmount - g_EdgeWidth, g_DissolveAmount, noiseValue);    
    
    // edgeFactor�� �̿��� ��� ���� ���� 
    // edgeFactor�� 1�̸� ��� ��(����), 0�̸� ��� �� (���� Ŭ��) 
    // ��� = edgeFactor�� 0~1 ���̸� ������ �κ�
    // (�� �� �����ϰ� �Ϸ��� saturate, lerp�� �̿�)
    
    //float  edgeStrength = 1.0 - edgeFactor; // ��� �αٿ��� 1
    //float4 edgeBlend = lerp(vMtrlDiffuse, g_EdgeColor, edgeStrength);    
    //float4 finalColor = lerp(vMtrlDiffuse, edgeBlend, edgeStrength);
    
    /* ////////////////////////////////// */
    
    float4 vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	
    /* ź��Ʈ �����̽��� �����ϴ� ����̴�. ���� (0~1 ) UnNormal�� ���� �Ǿ� ���� */  
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;

   /* ���� �����̽����� �븻�� ��ȯ����. */
    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
    
    vNormal = normalize(mul(vNormal, WorldMatrix));
    
    //Out.vDiffuse = vMtrlDiffuse;
    /* ������ ���� */
    float blendFactor = 0.5; // 0 ~ 1 ������ ��, ȿ���� ���� ����
    Out.vDiffuse = lerp(vMtrlDiffuse, finalColor, blendFactor);
    Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
    
    //Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);



    return Out;
}

PS_OUT_DISTORTION PS_MAIN_DISTORTION(PS_IN In)
{
    PS_OUT_DISTORTION Out = (PS_OUT_DISTORTION) 0;

    //�Ƹ� �̷� �۾��� �ʿ��Ұ���
    
    //vector vNoise = g_DistortionTexture.Sample(LinearSampler, In.vTexCoord * g_fTimer);
    //
    //float fWeight = vNoise.r * 0.9f;
    //
    //Out.vColor = g_NoiseTexture.Sample(LinearSampler, In.vTexCoord) * vNoise;
    
    //������ ���纻���̹Ƿ� �� �Ʒ��� �� ������������
    vector vNoise = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord * g_Time);
    
    float fWeight = vNoise.r * 0.9f;
    
    float2 vTexcoord = float2(In.vTexcoord.x * fWeight, In.vTexcoord.y);
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, vTexcoord) * vNoise;
    
    //������� ������������
    
	
    if (vMtrlDiffuse.a < 0.1f)
        discard;
	
    Out.vDistortion = vMtrlDiffuse;
	
    return Out;
}

PS_OUT_GLOW PS_MAIN_GLOW(PS_IN In)
{
    PS_OUT_GLOW Out = (PS_OUT_GLOW) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
	
    if (vMtrlDiffuse.a < 0.1f)
        discard;
	
    Out.vGlow = vMtrlDiffuse;
    
    vector vLook = normalize(In.vWorldPos - g_vCamPosition);
    
    
    
    float fDegree_Look_Normal = dot(vLook, In.vNormal);
        
    if (-0.4f < fDegree_Look_Normal && 0.4f > fDegree_Look_Normal)
        Out.vGlow.rgba = 1.f;
	
    return Out;
}

PS_OUT_MOTIONBLUR PS_MAIN_MOTIONBLUR(PS_IN_MOTIONBLUR In)
{
    PS_OUT_MOTIONBLUR Out = (PS_OUT_MOTIONBLUR) 0;
    
    Out.vMotionBlur.xy = In.vDir.xy * 10.f; //�ӵ��� ��Frame ���Ǽ� �ʹ� �۾Ƽ� �� �÷��� Deferred �� ������
    Out.vMotionBlur.z = In.vDir.w;
    Out.vMotionBlur.w = In.vDir.z / In.vDir.w;
	
    return Out;
}

PS_OUT PS_MAIN_OCCULUSION(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
   
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    
    if (vMtrlDiffuse.a >= 0.9f)
    {
        Out.vDiffuse = float4(0.f, 0.f, 0.f, 1.f);
        Out.vDepth = float4(0.f, 0.f, 0.f, 1.f);
        Out.vNormal = float4(0.f, 0.f, 0.f, 1.f);
    }
    else
    {
        Out.vDiffuse = float4(0.1f, 0.1f, 0.1f, 1.f);
        Out.vDepth = float4(1.f, 1.f, 1.f, 1.f);
        Out.vNormal = float4(1.f, 1.f, 1.f, 1.f);
    }
    return Out;
}


struct GS_IN_SHADOW
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};
struct GS_OUT_SHADOW
{
    float4 vPosition : SV_Position;
    float2 vTexcoord : TEXCOORD0;
    uint RTIndex : SV_RenderTargetArrayIndex;
};

[maxvertexcount(9)]
void GS_MAIN(triangle GS_IN_SHADOW In[3], inout TriangleStream<GS_OUT_SHADOW> DataStream)
{
    for (int face = 0; face < 3; ++face)
    {
   
        for (int i = 0; i < 3; ++i)
        {
            GS_OUT_SHADOW element;
        
            element.RTIndex = face;
            
//element.vPosition = mul(In[i].vPosition, mul(g_LightViewMatrix[face], g_LightProjMatrix[face]));
            element.vPosition = mul(In[i].vPosition, g_LightProjMatrix[face]);
            element.vTexcoord = In[i].vTexcoord;
            DataStream.Append(element);
        }
        
        DataStream.RestartStrip();
    }
}
struct PSIn
{
    float4 vPosition : SV_Position;
    float2 vTexcoord : TEXCOORD0;
};


struct PSOut
{
    float vColor : SV_TARGET0;
};

PSOut PSMainShadow(PSIn In)
{
    PSOut Out = (PSOut) 0;
    
    Out.vColor = In.vPosition.z;
    
    return Out;
}


technique11 DefaultTechnique
{
    pass DefaultPass //0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    
    pass DissolvePass //1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DISSOLVE();
    }

    pass Shadow //2 
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PSMainShadow();
    }

    pass Distortion //3
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DISTORTION();
    }

    pass Glow //4
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GLOW();
    }

    pass MotionBlur //5
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_MOTIONBLUR();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_MOTIONBLUR();
    }

    pass Occulsion //6
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_OCCULUSION();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_OCCULUSION();
    }
}
