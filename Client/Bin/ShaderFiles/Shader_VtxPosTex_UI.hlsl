#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D g_Texture;
Texture2D g_TexIcon;

bool g_bIconOn; // 아이템 슬롯 이미지 열고 닫기


float2 g_vTexcoord;

float g_fTimeDelta; // 인게임 업데이트 시간값
float g_fTImeAlpha; // 시간에 따른 알파 값

bool g_bImageOn; // True 값일 때 이미지 모습이 보임
bool g_bImageLoopOn; // true 값일 때 이미지 모습이 깜박깜박 보임 


struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    Out.vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vWorldPos = Out.vPosition;

    Out.vPosition = mul(Out.vPosition, g_ViewMatrix);
    Out.vPosition = mul(Out.vPosition, g_ProjMatrix);
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/* Sample : g_Texture로 부터 지정된 좌표의 색을 지정한 방식으로 얻어온다.*/
	// g_Texture.Sample(어떻게 얻어올건지(Sampler_State), 어디 색을 가져올건지)
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    
    //if (0.1f >= Out.vColor.a)
    //    discard;


    return Out;
}


PS_OUT PS_Thymesia_UI(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/* Sample : g_Texture로 부터 지정된 좌표의 색을 지정한 방식으로 얻어온다.*/
	// g_Texture.Sample(어떻게 얻어올건지(Sampler_State), 어디 색을 가져올건지)
    
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    

    return Out;
}

PS_OUT PS_Thymesia_UI_ImageOnOff(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/* Sample : g_Texture로 부터 지정된 좌표의 색을 지정한 방식으로 얻어온다.*/
	// g_Texture.Sample(어떻게 얻어올건지(Sampler_State), 어디 색을 가져올건지)
    
    if (true == g_bImageOn)
    {
        Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    }
    else
    {
        Out.vColor.a = 0.0f;

    }
    

    return Out;
}

PS_OUT PS_Thymesia_UI_ImageOnOffLoop(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    if (true == g_bImageLoopOn) // 루프 값이 켜졌을 때
    {
        Out.vColor.rgb *= abs(g_fTImeAlpha);

    }
    return Out;
}

PS_OUT PS_Thymesia_UI_Image_ItemSlot(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    float4 vBackColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    float4 vFrontColor = g_TexIcon.Sample(LinearSampler, In.vTexcoord);
    
    if (!g_bIconOn)
        vFrontColor.a = 0.0f;
    
    Out.vColor = lerp(vBackColor, vFrontColor, vFrontColor.a);
    
    return Out;
}
PS_OUT PS_Thymesia_UI_Image_Pause(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    Out.vColor.a -= 0.6f;
    return Out;
}

technique11 DefaultTechnique
{
  // 0번
    pass DefaultPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Thymesia_UI // 1번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI();
    }

    pass Thymesia_UI_ImageOnOff // 2번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_ImageOnOff();
    }

    pass Thymesia_UI_ImageOnOffLoop // 3번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_ImageOnOffLoop();
    }

    pass Thymesia_UI_Image_ItemSlot // 4번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_ItemSlot();
    }
    
    pass Thymesia_UI_Image_Pause // 5번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_Pause();
    }
    

}
