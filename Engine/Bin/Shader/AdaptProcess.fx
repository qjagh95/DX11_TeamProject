#include "ComputeProcess.fx"

Texture2D HDRed_Tex : register(t9);

StructuredBuffer<float> AverageValues1D_2rd : register(t9);
StructuredBuffer<float> PrevAvgLum : register(t10);

RWStructuredBuffer<float> AverageLum2 : register(u0);

cbuffer DownScaleAdaptation : register(b3)
{
    float4 vAdaptation;
}

groupshared float SharedAdaptPositions[1024];

static const float4 LUM_ADAPT_FACTOR = float4(0.299, 0.587, 0.114, 0);

[numthreads(1024, 1, 1)]
void DownScaleAdaptationFirstPass(uint3 groupId : SV_GroupID, uint3 groupThreadId : SV_GroupThreadID,
    uint3 dispatchThreadId : SV_DispatchThreadID)
{
    uint2 vCurPixel = uint2(dispatchThreadId.x % g_Res.x,
    dispatchThreadId.x / g_Res.x);
    
    float favgLum = DownScale4x4(vCurPixel, groupThreadId.x);

     // 1024에서 4로 다운스케일
    favgLum = DownScale1024to4(dispatchThreadId.x, groupThreadId.x,
         favgLum);

    // 4에서 1로 다운스케일
    DownScale4to1(dispatchThreadId.x, groupThreadId.x, groupId.x,
        favgLum);
}

// Group shared memory to store the intermidiate results
groupshared float SharedAdaptAvgFinal[MAX_GROUPS];

[numthreads(MAX_GROUPS, 1, 1)]
void DownScaleAdaptationSecondPass(uint3 groupId : SV_GroupID, uint3 groupThreadId : SV_GroupThreadID,
    uint3 dispatchThreadId : SV_DispatchThreadID)
{
    // 공유 메모리에 ID값 저장
    float favgLum = 0.f;

    if (dispatchThreadId.x < g_GroupSize)
    {
        favgLum = AverageValues1D[dispatchThreadId.x];
    }

    SharedAvgFinal[dispatchThreadId.x] = favgLum;

    GroupMemoryBarrierWithGroupSync(); // 동기화 후 다음 과정으로

    // 64에서 16으로 다운 스케일
    if (dispatchThreadId.x % 4 == 0)
    {
        // 휘도 값 합산
        float fstepAvgLum = favgLum;

        fstepAvgLum += dispatchThreadId.x + 1 < g_GroupSize ?
            SharedAvgFinal[dispatchThreadId.x + 1] : favgLum;

        fstepAvgLum += dispatchThreadId.x + 2 < g_GroupSize ?
            SharedAvgFinal[dispatchThreadId.x + 2] : favgLum;

        fstepAvgLum += dispatchThreadId.x + 3 < g_GroupSize ?
            SharedAvgFinal[dispatchThreadId.x + 3] : favgLum;

        // 결과 값 저장
        favgLum = fstepAvgLum;
        SharedAvgFinal[dispatchThreadId.x] = fstepAvgLum;
    }

    GroupMemoryBarrierWithGroupSync(); // 동기화 후 다음 과정으로

    // 16에서 4로 다운스케일
    if (dispatchThreadId.x % 16 == 0)
    {
        // 휘도 값 합산
        float fstepAvgLum = favgLum;

        fstepAvgLum += dispatchThreadId.x + 4 < g_GroupSize ?
            SharedAvgFinal[dispatchThreadId.x + 4] : favgLum;

        fstepAvgLum += dispatchThreadId.x + 8 < g_GroupSize ?
            SharedAvgFinal[dispatchThreadId.x + 8] : favgLum;

        fstepAvgLum += dispatchThreadId.x + 12 < g_GroupSize ?
            SharedAvgFinal[dispatchThreadId.x + 12] : favgLum;

        // 결과 값 저장
        favgLum = fstepAvgLum;
        SharedAvgFinal[dispatchThreadId.x] = fstepAvgLum;
    }
    
    GroupMemoryBarrierWithGroupSync(); // 동기화 후 다음 과정으로

    // 4에서 1로 다운스케일
    if (dispatchThreadId.x == 0)
    {
        // 휘도 값 합산
        float fFinalLumValue = favgLum;

        fFinalLumValue += dispatchThreadId.x + 16 < g_GroupSize ?
            SharedAvgFinal[dispatchThreadId.x + 16] : favgLum;

        fFinalLumValue += dispatchThreadId.x + 32 < g_GroupSize ?
            SharedAvgFinal[dispatchThreadId.x + 32] : favgLum;

        fFinalLumValue += dispatchThreadId.x + 48 < g_GroupSize ?
            SharedAvgFinal[dispatchThreadId.x + 48] : favgLum;

        fFinalLumValue /= 64.f;

        // Adaptive 휘도를 계산한다.
        float fApaptedAverageLum = lerp(PrevAvgLum[0], fFinalLumValue, vAdaptation);

        AverageLum[0] = max(fApaptedAverageLum, 0.0001);
    }
}