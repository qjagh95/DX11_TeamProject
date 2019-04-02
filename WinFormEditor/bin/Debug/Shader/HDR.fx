#include "ComputeShare.fx"

Texture2D HDRTex : register(t0);

StructuredBuffer<float> AverageValues1D : register(t1);
StructuredBuffer<float> PrevAvgLum : register(t2);

//RWStructuredBuffer<float> AverageLum : register(u0);
RWStructuredBuffer<float> AverageLumFinal : register(u0);

groupshared float SharedPositions[1024];

cbuffer Adapt_Bloom_CB : register(b3)
{
    float   g_fAdaptation;
    float3  vEmpty;
}

cbuffer BloomThresholdCB : register(b4)
{
    float g_fBloomThreshold;
    float3 g_vEmpty;
}

// �� �����忡 ���� 4x4 �ٿ� �������� �����Ѵ�
float4 DownScale4x4(uint2 CurPixel, uint groupThreadId)
{
    float avgLum = 0.f;

    if (CurPixel.y < g_Res.y)
    {
        int3 iFullResPos = int3(CurPixel * 4, 0);
        float4 vDownScaled = float4(0.f, 0.f, 0.f, 0.f);

        [unroll]
        for (int i = 0; i < 4; ++i)
        {
            [unroll]
            for (int j = 0; j < 4; ++j)
            {
                vDownScaled += HDRTex.Load(iFullResPos, int2(j, i));

            }
        }
        vDownScaled /= 16;
        
        // �ȼ��� �ֵ� �� ���
        avgLum = dot(vDownScaled, LUM_FACTOR);

        // ���� �޸𸮿� ��� ���
        SharedPositions[groupThreadId] = avgLum;
    }

    // ����ȭ �� ���� �ܰ��
    GroupMemoryBarrierWithGroupSync();

    return avgLum;
}

// ������ ���� ���� 4���� ������ �ٿ�����Ѵ�
float DownScale1024to4(uint dispachThreadId, uint groupThreadId,
    float avgLum)
{
    // �ٿ���� �ڵ带 Ȯ��
    for (uint iGroupSize = 4, iStep1 = 1, iStep2 = 2, iStep3 = 3;
        iGroupSize < 1024;
        iGroupSize *= 4, iStep1 *= 4, iStep2 *= 4, iStep3 *= 4)
    {
        if (groupThreadId % iGroupSize == 0)
        {
            float fStepAvgLum = avgLum;

            fStepAvgLum += dispachThreadId + iStep1 < g_Domain ?
                SharedPositions[groupThreadId + iStep1] : avgLum;

            fStepAvgLum += dispachThreadId + iStep2 < g_Domain ?
                SharedPositions[groupThreadId + iStep2] : avgLum;

            fStepAvgLum += dispachThreadId + iStep3 < g_Domain ?
              SharedPositions[groupThreadId + iStep3] : avgLum;

            // ��� �� ����
            avgLum = fStepAvgLum;
            SharedPositions[groupThreadId] = fStepAvgLum;
        }
        // ����ȭ �� ��������
        GroupMemoryBarrierWithGroupSync();
    }
    return avgLum;
}

// 4���� ���� �ϳ��� ��հ����� �ٿ������ �� �����Ѵ�
void DownScale4to1(uint dispatchThreadId, uint groupThreadId,
    uint groupId, float avgLum)
{
    if (groupThreadId == 0)
    {
        //  ������ �׷쿡 ���� ��� �ֵ� �� ���
        float fFinalAvgLum = avgLum;

        fFinalAvgLum += dispatchThreadId + 256 < g_Domain ?
            SharedPositions[groupThreadId + 256] : avgLum;

        fFinalAvgLum += dispatchThreadId + 512 < g_Domain ?
            SharedPositions[groupThreadId + 512] : avgLum;

        fFinalAvgLum += dispatchThreadId + 768 < g_Domain ?
            SharedPositions[groupThreadId + 768] : avgLum;

        fFinalAvgLum /= 1024.f;

        // ���� ���� ID UAV�� ��� �� ���� ��������
        AverageLumFinal[groupId] = fFinalAvgLum;
    }
}

// �̷��� ���� ���� ���̴� ��Ʈ�� ����Ʈ�� ���Եȴ�
[numthreads(1024, 1, 1)]
void DownScaleFirstPass(uint3 groupId : SV_GroupID,
    uint3 dispatchThreadId : SV_DispatchThreadID,
    uint3 groupThreadId : SV_GroupThreadID)
{
    uint2 vCurPixel = uint2(dispatchThreadId.x % g_Res.x,
    dispatchThreadId.x / g_Res.x);

    // 16 �ȼ� �׷��� �ϳ��� �ȼ��� �ٿ� ���� �޸𸮿� ����
    float favgLum = DownScale4x4(vCurPixel, groupThreadId.x);

    // 1024���� 4�� �ٿ����
    favgLum = DownScale1024to4(dispatchThreadId.x, groupThreadId.x,
         favgLum);

    // 4���� 1�� �ٿ����
    DownScale4to1(dispatchThreadId.x, groupThreadId.x, groupId.x,
        favgLum);

    // �� ��ǻƮ ���̴��� x �� (������� �� �ȼ� �� / (16 * 1024)) �� ���� ��� ó���� �� �ִ�
}

// ù ��° ��ǻƮ ���̴��� ������ �Ϸ�Ǹ� ������ ��� ���۸� ����� �ι�° ��ǻƮ ���̴��� �����Ѵ�
// �߰� �� �ֵ� SRV�� ��� �ֵ� UAV ���� ������ ����Ѵ�
#define MAX_GROUPS 64

// ���� �޸� �׷쿡 �߰� �� ����
groupshared float SharedAvgFinal[MAX_GROUPS];

[numthreads(MAX_GROUPS, 1, 1)]
void DownScaleSecondPass(uint3 groupId : SV_GroupID,
        uint3 groupThreadId : SV_GroupThreadID,
        uint3 dispatchThreadId : SV_DispatchThreadID)
{
    // ���� �޸𸮿� ID�� ����
    float favgLum = 0.f;

    if (dispatchThreadId.x < g_GroupSize)
    {
        favgLum = AverageValues1D[dispatchThreadId.x];
    }

    SharedAvgFinal[dispatchThreadId.x] = favgLum;

    GroupMemoryBarrierWithGroupSync(); // ����ȭ �� ���� ��������

    // 64���� 16���� �ٿ� ������
    if (dispatchThreadId.x % 4 == 0)
    {
        // �ֵ� �� �ջ�
        float fstepAvgLum = favgLum;

        fstepAvgLum += dispatchThreadId.x + 1 < g_GroupSize ?
            SharedAvgFinal[dispatchThreadId.x + 1] : favgLum;

        fstepAvgLum += dispatchThreadId.x + 2 < g_GroupSize ?
            SharedAvgFinal[dispatchThreadId.x + 2] : favgLum;

        fstepAvgLum += dispatchThreadId.x + 3 < g_GroupSize ?
            SharedAvgFinal[dispatchThreadId.x + 3] : favgLum;

        // ��� �� ����
        favgLum = fstepAvgLum;

        SharedAvgFinal[dispatchThreadId.x] = fstepAvgLum;
    }

    GroupMemoryBarrierWithGroupSync(); // ����ȭ �� ���� ��������

    // 16���� 4�� �ٿ����
    if (dispatchThreadId.x % 16 == 0)
    {
        // �ֵ� �� �ջ�
        float fstepAvgLum = favgLum;

        fstepAvgLum += dispatchThreadId.x + 4 < g_GroupSize ?
            SharedAvgFinal[dispatchThreadId.x + 4] : favgLum;

        fstepAvgLum += dispatchThreadId.x + 8 < g_GroupSize ?
            SharedAvgFinal[dispatchThreadId.x + 8] : favgLum;

        fstepAvgLum += dispatchThreadId.x + 12 < g_GroupSize ?
            SharedAvgFinal[dispatchThreadId.x + 12] : favgLum;

        // ��� �� ����
        favgLum = fstepAvgLum;
        SharedAvgFinal[dispatchThreadId.x] = fstepAvgLum;
    }
    
    GroupMemoryBarrierWithGroupSync(); // ����ȭ �� ���� ��������

    // 4���� 1�� �ٿ����
    if (dispatchThreadId.x == 0)
    {
        // �ֵ� �� �ջ�
        float fFinalLumValue = favgLum;

        fFinalLumValue += dispatchThreadId.x + 16 < g_GroupSize ?
            SharedAvgFinal[dispatchThreadId.x + 16] : favgLum;

        fFinalLumValue += dispatchThreadId.x + 32 < g_GroupSize ?
            SharedAvgFinal[dispatchThreadId.x + 32] : favgLum;

        fFinalLumValue += dispatchThreadId.x + 48 < g_GroupSize ?
            SharedAvgFinal[dispatchThreadId.x + 48] : favgLum;

        fFinalLumValue /= 64.f;

        float fApaptedAverageLum = lerp(PrevAvgLum[1], fFinalLumValue, g_fAdaptation);

        AverageLumFinal[0] = max(fFinalLumValue, 0.0001);
       // AverageLumFinal[1] = max(fApaptedAverageLum, 0.0001);

    }
}

// Bloom Compute Shader
Texture2D<float4> HDRDownScaleTex : register(t0);
StructuredBuffer<float> AvgLum : register(t1);

RWTexture2D<float4> Bloom : register(u0);

[numthreads(1024, 1, 1)]
void BloomReveal(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint2 vCurPixel = uint2(dispatchThreadID.x % g_Res.x,
    dispatchThreadID.x / g_Res.x);

    if (vCurPixel.y < g_Res.y)
    {
        float4 vColor = HDRDownScaleTex.Load(int3(vCurPixel, 0));
        float fLum = dot(vColor, LUM_FACTOR);
        float fAvgLum = AvgLum[0];

        float fColorScale = saturate(fLum - fAvgLum * g_fBloomThreshold);

        Bloom[vCurPixel.xy] = vColor * fColorScale;
    }
}