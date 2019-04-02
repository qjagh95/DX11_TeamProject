#include "ComputeShare.fx"

Texture2D HDRed_Tex : register(t0);

StructuredBuffer<float> AverageValues1D_2rd : register(t1);
StructuredBuffer<float> PrevAvgLum : register(t2);

RWStructuredBuffer<float> AverageLum2 : register(u0);

cbuffer Adapt_Bloom_CB : register(b3)
{
    float g_fAdaptation;
    float3 vEmpty;
}

cbuffer BloomThresholdCB : register(b4)
{
    float g_fBloomThreshold;
    float3 g_vEmpty;
}

groupshared float SharedAdaptBloomPositions[1024];

static const float4 LUM_ADAPT_FACTOR = float4(0.299, 0.587, 0.114, 0);

[numthreads(1024, 1, 1)]
void DownScaleAdaptationFirstPass(uint3 groupID : SV_GroupID, uint3 groupThreadID : SV_GroupThreadID,
    uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint2 vCurPixel = uint2(dispatchThreadID.x % g_Res.x,
    dispatchThreadID.x / g_Res.x);

    float fAvgLum = 0.f;
    if (vCurPixel.y < g_Res.y)
    {
        int3 vFullResPos = int3(vCurPixel * 4, 0);
        float4 vDownScaled = float4(0.f, 0.f, 0.f, 0.f);
        
        [unroll]
        for (int i = 0; i < 4; ++i)
        {
            [unroll]
            for (int j = 0; j < 4; ++j)
            {
                vDownScaled += HDRed_Tex.Load(vFullResPos, int2(j, i));
            }
        }

        vDownScaled /= 16.f;
        //HDRDownScale[vCurPixel.xy] = vDownScaled;
        fAvgLum = dot(vDownScaled, LUM_FACTOR);
    }

    SharedAdaptBloomPositions[groupThreadID.x] = fAvgLum;

    GroupMemoryBarrierWithGroupSync();
    
	// Down scale from 1024 to 256
    if (groupThreadID.x % 4 == 0)
    {
		// Calculate the luminance sum for this step
        float fStepAvgLum = fAvgLum;
        fStepAvgLum += dispatchThreadID.x + 1 < g_Domain ? SharedAdaptBloomPositions[groupThreadID.x + 1] : fAvgLum;
        fStepAvgLum += dispatchThreadID.x + 2 < g_Domain ? SharedAdaptBloomPositions[groupThreadID.x + 2] : fAvgLum;
        fStepAvgLum += dispatchThreadID.x + 3 < g_Domain ? SharedAdaptBloomPositions[groupThreadID.x + 3] : fAvgLum;
		
		// Store the results
        fAvgLum = fStepAvgLum;
        SharedAdaptBloomPositions[groupThreadID.x] = fStepAvgLum;
    }

    GroupMemoryBarrierWithGroupSync(); // Sync before next step

	// Downscale from 256 to 64
    if (groupThreadID.x % 16 == 0)
    {
		// Calculate the luminance sum for this step
        float fStepAvgLum = fAvgLum;
        fStepAvgLum += dispatchThreadID.x + 4 < g_Domain ? SharedAdaptBloomPositions[groupThreadID.x + 4] : fAvgLum;
        fStepAvgLum += dispatchThreadID.x + 8 < g_Domain ? SharedAdaptBloomPositions[groupThreadID.x + 8] : fAvgLum;
        fStepAvgLum += dispatchThreadID.x + 12 < g_Domain ? SharedAdaptBloomPositions[groupThreadID.x + 12] : fAvgLum;

		// Store the results
        fAvgLum = fStepAvgLum;
        SharedAdaptBloomPositions[groupThreadID.x] = fStepAvgLum;
    }

    GroupMemoryBarrierWithGroupSync(); // Sync before next step

	// Downscale from 64 to 16
    if (groupThreadID.x % 64 == 0)
    {
		// Calculate the luminance sum for this step
        float fStepAvgLum = fAvgLum;
        fStepAvgLum += dispatchThreadID.x + 16 < g_Domain ? SharedAdaptBloomPositions[groupThreadID.x + 16] : fAvgLum;
        fStepAvgLum += dispatchThreadID.x + 32 < g_Domain ? SharedAdaptBloomPositions[groupThreadID.x + 32] : fAvgLum;
        fStepAvgLum += dispatchThreadID.x + 48 < g_Domain ? SharedAdaptBloomPositions[groupThreadID.x + 48] : fAvgLum;

		// Store the results
        fAvgLum = fStepAvgLum;
        SharedAdaptBloomPositions[groupThreadID.x] = fStepAvgLum;
    }

    GroupMemoryBarrierWithGroupSync(); // Sync before next step

	// Downscale from 16 to 4
    if (groupThreadID.x % 256 == 0)
    {
		// Calculate the luminance sum for this step
        float fStepAvgLum = fAvgLum;
        fStepAvgLum += dispatchThreadID.x + 64 < g_Domain ? SharedAdaptBloomPositions[groupThreadID.x + 64] : fAvgLum;
        fStepAvgLum += dispatchThreadID.x + 128 < g_Domain ? SharedAdaptBloomPositions[groupThreadID.x + 128] : fAvgLum;
        fStepAvgLum += dispatchThreadID.x + 192 < g_Domain ? SharedAdaptBloomPositions[groupThreadID.x + 192] : fAvgLum;

		// Store the results
        fAvgLum = fStepAvgLum;
        SharedAdaptBloomPositions[groupThreadID.x] = fStepAvgLum;
    }

    GroupMemoryBarrierWithGroupSync(); // Sync before next step

	// Downscale from 4 to 1
    if (groupThreadID.x == 0)
    {
		// Calculate the average lumenance for this thread group
        float fFinalAvgLum = fAvgLum;
        fFinalAvgLum += dispatchThreadID.x + 256 < g_Domain ? SharedAdaptBloomPositions[groupThreadID.x + 256] : fAvgLum;
        fFinalAvgLum += dispatchThreadID.x + 512 < g_Domain ? SharedAdaptBloomPositions[groupThreadID.x + 512] : fAvgLum;
        fFinalAvgLum += dispatchThreadID.x + 768 < g_Domain ? SharedAdaptBloomPositions[groupThreadID.x + 768] : fAvgLum;
        fFinalAvgLum /= 1024.0;

        AverageLum2[groupID.x] = fFinalAvgLum; // Write the final value into the 1D UAV which will be used on the next step
    }
}

#define MAX_GROUPS 64

// Group shared memory to store the intermidiate results
groupshared float SharedAdaptBloomAvgFinal[MAX_GROUPS];

[numthreads(MAX_GROUPS, 1, 1)]
void DownScaleAdaptationSecondPass(uint3 groupId : SV_GroupID, uint3 groupThreadId : SV_GroupThreadID,
    uint3 dispatchThreadId : SV_DispatchThreadID)
{
    // 공유 메모리에 ID값 저장
    float favgLum = 0.f;

    if (dispatchThreadId.x < g_GroupSize)
    {
        favgLum = AverageValues1D_2rd[dispatchThreadId.x];
    }

    SharedAdaptBloomAvgFinal[dispatchThreadId.x] = favgLum;

    GroupMemoryBarrierWithGroupSync(); // 동기화 후 다음 과정으로

    // 64에서 16으로 다운 스케일
    if (dispatchThreadId.x % 4 == 0)
    {
        // 휘도 값 합산
        float fstepAvgLum = favgLum;

        fstepAvgLum += dispatchThreadId.x + 1 < g_GroupSize ?
            SharedAdaptBloomAvgFinal[dispatchThreadId.x + 1] : favgLum;

        fstepAvgLum += dispatchThreadId.x + 2 < g_GroupSize ?
            SharedAdaptBloomAvgFinal[dispatchThreadId.x + 2] : favgLum;

        fstepAvgLum += dispatchThreadId.x + 3 < g_GroupSize ?
            SharedAdaptBloomAvgFinal[dispatchThreadId.x + 3] : favgLum;

        // 결과 값 저장
        favgLum = fstepAvgLum;
        SharedAdaptBloomAvgFinal[dispatchThreadId.x] = fstepAvgLum;
    }

    GroupMemoryBarrierWithGroupSync(); // 동기화 후 다음 과정으로

    // 16에서 4로 다운스케일
    if (dispatchThreadId.x % 16 == 0)
    {
        // 휘도 값 합산
        float fstepAvgLum = favgLum;

        fstepAvgLum += dispatchThreadId.x + 4 < g_GroupSize ?
            SharedAdaptBloomAvgFinal[dispatchThreadId.x + 4] : favgLum;

        fstepAvgLum += dispatchThreadId.x + 8 < g_GroupSize ?
            SharedAdaptBloomAvgFinal[dispatchThreadId.x + 8] : favgLum;

        fstepAvgLum += dispatchThreadId.x + 12 < g_GroupSize ?
            SharedAdaptBloomAvgFinal[dispatchThreadId.x + 12] : favgLum;

        // 결과 값 저장
        favgLum = fstepAvgLum;
        SharedAdaptBloomAvgFinal[dispatchThreadId.x] = fstepAvgLum;
    }
    
    GroupMemoryBarrierWithGroupSync(); // 동기화 후 다음 과정으로

    // 4에서 1로 다운스케일
    if (dispatchThreadId.x == 0)
    {
        // 휘도 값 합산
        float fFinalLumValue = favgLum;

        fFinalLumValue += dispatchThreadId.x + 16 < g_GroupSize ?
            SharedAdaptBloomAvgFinal[dispatchThreadId.x + 16] : favgLum;

        fFinalLumValue += dispatchThreadId.x + 32 < g_GroupSize ?
            SharedAdaptBloomAvgFinal[dispatchThreadId.x + 32] : favgLum;

        fFinalLumValue += dispatchThreadId.x + 48 < g_GroupSize ?
            SharedAdaptBloomAvgFinal[dispatchThreadId.x + 48] : favgLum;

        fFinalLumValue /= 64.f;

        // Adaptive 휘도를 계산한다.
        float fApaptedAverageLum = lerp(PrevAvgLum[0], fFinalLumValue, g_fAdaptation);

        AverageLum2[0] = max(fApaptedAverageLum, 0.0001);
    }
}

// Bloom Compute Shader
Texture2D<float4> HDRDownScaleTex : register(t0);
StructuredBuffer<float> AvgLum : register(t1);

RWTexture2D<float4> Bloom : register(u1);

[numthreads(1024, 1, 1)]
void BloomReveal(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint2 vCurPixel = uint2(dispatchThreadID.x % g_Res.x,
    dispatchThreadID.x / g_Res.x);

    if (vCurPixel.y < g_Res.y)
    {
        float4 vColor = HDRDownScaleTex.Load(int3(vCurPixel, 0));
        float fLum = dot(vColor, LUM_ADAPT_FACTOR);
        float fAvgLum = AvgLum[0];

        float fColorScale = saturate(fLum - fAvgLum * g_fBloomThreshold);

        Bloom[vCurPixel.xy] = vColor * fColorScale;
    }
}

// Bloom용 Blur Shader입니다. 
//얘는 Depth of Field 처럼 깊이 값으로 조절하진 못하고
// 그냥 ㄹㅇ 그냥 Blur입니다.
// 대충 blur를 써야하는 상황이 생긴다면 요걸 쓰는게 좋습니다.
Texture2D<float4> Input : register(t0);
RWTexture2D<float4> Output : register(u0);

static const float SampleWeights[13] =
{
    0.002216,
    0.008764,
    0.026995,
    0.064759,
    0.120985,
    0.176033,
    0.199471,
    0.176033,
    0.120985,
    0.064759,
    0.026995,
    0.008764,
    0.002216,
};

#define kernelhalf 6
#define groupthreads 128
groupshared float4 SharedInput[groupthreads];

[numthreads(groupthreads, 1, 1)]
void VerticalFilter(uint3 GroupID : SV_GroupID, uint GroupIndex : SV_GroupIndex)
{
    int2 vCoord = int2(GroupID.x, GroupIndex - kernelhalf + (groupthreads - kernelhalf * 2) * GroupID.y);
    vCoord = clamp(vCoord, int2(0, 0), int2(g_Res.x - 1, g_Res.y - 1));
    SharedInput[GroupIndex] = Input.Load(int3(vCoord, 0));

    GroupMemoryBarrierWithGroupSync();

    // Vertical blur
    if (GroupIndex >= kernelhalf && GroupIndex < (groupthreads - kernelhalf) &&
         ((GroupIndex - kernelhalf + (groupthreads - kernelhalf * 2) * GroupID.y) < g_Res.y))
    {
        float4 vOut = 0;
        
        [unroll]
        for (int i = -kernelhalf; i <= kernelhalf; ++i)
        {
            vOut += SharedInput[GroupIndex + i] * SampleWeights[i + kernelhalf];
        }

        Output[vCoord] = float4(vOut.rgb, 1.0f);
    }
}

[numthreads(groupthreads, 1, 1)]
void HorizFilter(uint3 GroupID : SV_GroupID, uint GroupIndex : SV_GroupIndex)
{
    int2 vCoord = int2(GroupIndex - kernelhalf + (groupthreads - kernelhalf * 2) * GroupID.x, GroupID.y);
    vCoord = clamp(vCoord, int2(0, 0), int2(g_Res.x - 1, g_Res.y - 1));
    SharedInput[GroupIndex] = Input.Load(int3(vCoord, 0));

    GroupMemoryBarrierWithGroupSync();

    // Horizontal blur
    if (GroupIndex >= kernelhalf && GroupIndex < (groupthreads - kernelhalf) &&
         ((GroupID.x * (groupthreads - 2 * kernelhalf) + GroupIndex - kernelhalf) < g_Res.x))
    {
        float4 vOut = 0;
        
        [unroll]
        for (int i = -kernelhalf; i <= kernelhalf; ++i)
            vOut += SharedInput[GroupIndex + i] * SampleWeights[i + kernelhalf];

        Output[vCoord] = float4(vOut.rgb, 1.0f);
    }
}