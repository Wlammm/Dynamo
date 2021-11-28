#include "FullscreenStructs.hlsli"

float rgb2luma(float3 rgb)
{
    return sqrt(dot(rgb, float3(0.299f, 0.587f, 0.114f)));
}

static const int ITERATIONS = 12;
static const float QUALITY[ITERATIONS] = { 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 1.5f, 2.0f, 2.0f, 2.0f, 2.0f, 4.0f, 8.0f };
static const float SUBPIXEL_QUALITY = 0.75f;

cbuffer FXAABuffer : register(b8)
{
    struct FXAABuffer
    {
        float2 myResolution;

        float2 padding;
    } myFXAABuffer;
}

PixelOutput main(VertexToPixel input)
{
    PixelOutput result;
    float EDGE_THRESHOLD_MAX = 0.125;
    float EDGE_THRESHOLD_MIN = 0.0312;
    float2 inverseScreenSize = float2(1.0f / myFXAABuffer.myResolution.x, 1.0f / myFXAABuffer.myResolution.y);
    
    //Sample the fullscreen texture
    float3 colorCenter = myFullscreenTexture1.Sample(myDefaultSampler, input.myUV).rgb;
    
    //convert to luma
    float lumaCenter = rgb2luma(colorCenter);
    
    //Detecting where to apply AA First, edges need to be detected
    float lumaDown = rgb2luma(myFullscreenTexture1.Sample(myDefaultSampler, input.myUV, int2(0, -1)).rgb);
    float lumaUp = rgb2luma(myFullscreenTexture1.Sample(myDefaultSampler, input.myUV, int2(0, 1)).rgb);
    float lumaLeft = rgb2luma(myFullscreenTexture1.Sample(myDefaultSampler, input.myUV, int2(-1, 0)).rgb);
    float lumaRight = rgb2luma(myFullscreenTexture1.Sample(myDefaultSampler, input.myUV, int2(1, 0)).rgb);
    
    float lumaMin = min(lumaCenter, min(min(lumaDown, lumaUp), min(lumaLeft, lumaRight)));
    float lumaMax = max(lumaCenter, max(max(lumaDown, lumaUp), max(lumaLeft, lumaRight)));
    
    float lumaRange = lumaMax - lumaMin;
    
    // If the luma variation is lower that a threshold (or if we are in a really dark area), 
    // we are not on an edge, don't perform any AA.
    if (lumaRange < max(EDGE_THRESHOLD_MIN, lumaMax * EDGE_THRESHOLD_MAX))
    {
        result.myColor.rgb = colorCenter;
        return result;
    }
    
    // Query the 4 remaining corners lumas.
    float lumaDownLeft = rgb2luma(myFullscreenTexture1.Sample(myDefaultSampler, input.myUV, int2(-1, -1)).rgb);
    float lumaUpRight = rgb2luma(myFullscreenTexture1.Sample(myDefaultSampler, input.myUV, int2(1, 1)).rgb);
    float lumaUpLeft = rgb2luma(myFullscreenTexture1.Sample(myDefaultSampler, input.myUV, int2(-1, 1)).rgb);
    float lumaDownRight = rgb2luma(myFullscreenTexture1.Sample(myDefaultSampler, input.myUV, int2(1, -1)).rgb);
    
    float lumaDownUp = lumaDown + lumaUp;
    float lumaLeftRight = lumaLeft + lumaRight;
    
    float lumaLeftCorners = lumaDownLeft + lumaUpLeft;
    float lumaDownCorners = lumaDownLeft + lumaDownRight;
    float lumaRightCorners = lumaDownRight + lumaUpRight;
    float lumaUpCorners = lumaUpRight + lumaUpLeft;
    
    float edgeHorizontal = abs(-2.0 * lumaLeft + lumaLeftCorners) + abs(-2.0 * lumaCenter + lumaDownUp) * 2.0 + abs(-2.0 * lumaRight + lumaRightCorners);
    float edgeVertical = abs(-2.0 * lumaUp + lumaUpCorners) + abs(-2.0 * lumaCenter + lumaLeftRight) * 2.0 + abs(-2.0 * lumaDown + lumaDownCorners);
    
    bool isHorizontal = (edgeHorizontal >= edgeVertical);
    
    float luma1 = isHorizontal ? lumaDown : lumaLeft;
    float luma2 = isHorizontal ? lumaUp : lumaRight;
    
    float gradient1 = luma1 - lumaCenter;
    float gradient2 = luma2 - lumaCenter;

    bool is1Steepest = abs(gradient1) >= abs(gradient2);

    float gradientScaled = 0.25 * max(abs(gradient1), abs(gradient2));
    
    float stepLength = isHorizontal ? inverseScreenSize.y : inverseScreenSize.x;

    float lumaLocalAverage = 0.0;

    if (is1Steepest)
    {
        // Switch the direction
        stepLength = -stepLength;
        lumaLocalAverage = 0.5 * (luma1 + lumaCenter);
    }
    else
    {
        lumaLocalAverage = 0.5 * (luma2 + lumaCenter);
    }

    float2 currentUv = input.myUV;
    if (isHorizontal)
    {
        currentUv.y += stepLength * 0.5;
    }
    else
    {
        currentUv.x += stepLength * 0.5;
    }
    
    float2 offset = isHorizontal ? float2(inverseScreenSize.x, 0.0) : float2(0.0, inverseScreenSize.y);
    
    float2 uv1 = currentUv - offset;
    float2 uv2 = currentUv + offset;

    float lumaEnd1 = rgb2luma(myFullscreenTexture1.Sample(myDefaultSampler, uv1).rgb);
    float lumaEnd2 = rgb2luma(myFullscreenTexture1.Sample(myDefaultSampler, uv2).rgb);
    lumaEnd1 -= lumaLocalAverage;
    lumaEnd2 -= lumaLocalAverage;

    bool reached1 = abs(lumaEnd1) >= gradientScaled;
    bool reached2 = abs(lumaEnd2) >= gradientScaled;
    bool reachedBoth = reached1 && reached2;

    if (!reached1)
    {
        uv1 -= offset;
    }
    if (!reached2)
    {
        uv2 += offset;
    }
    
    if (!reachedBoth)
    {

        for (int i = 2; i < ITERATIONS; i++)
        {
            // If needed, read luma in 1st direction, compute delta.
            if (!reached1)
            {
                lumaEnd1 = rgb2luma(myFullscreenTexture1.Sample(myDefaultSampler, uv1).rgb);
                lumaEnd1 = lumaEnd1 - lumaLocalAverage;
            }
            
            // If needed, read luma in opposite direction, compute delta.
            if (!reached2)
            {
                lumaEnd2 = rgb2luma(myFullscreenTexture1.Sample(myDefaultSampler, uv2).rgb);
                lumaEnd2 = lumaEnd2 - lumaLocalAverage;
            }
            
            // If the luma deltas at the current extremities is larger than the local gradient, we have reached the side of the edge.
            reached1 = abs(lumaEnd1) >= gradientScaled;
            reached2 = abs(lumaEnd2) >= gradientScaled;
            reachedBoth = reached1 && reached2;

            // If the side is not reached, we continue to explore in this direction, with a variable quality.
            if (!reached1)
            {
                uv1 -= offset * QUALITY[i];
            }
            if (!reached2)
            {
                uv2 += offset * QUALITY[i];
            }

            // If both sides have been reached, stop the exploration.
            if (reachedBoth)
            {
                break;
            }
        }
    }
    
    float distance1 = isHorizontal ? (input.myUV.x - uv1.x) : (input.myUV.y - uv1.y);
    float distance2 = isHorizontal ? (uv2.x - input.myUV.x) : (uv2.y - input.myUV.y);

    bool isDirection1 = distance1 < distance2;
    float distanceFinal = min(distance1, distance2);

    float edgeThickness = (distance1 + distance2);

    float pixelOffset = -distanceFinal / edgeThickness + 0.5;
    
    bool isLumaCenterSmaller = lumaCenter < lumaLocalAverage;
    bool correctVariation = ((isDirection1 ? lumaEnd1 : lumaEnd2) < 0.0) != isLumaCenterSmaller;

    float finalOffset = correctVariation ? pixelOffset : 0.0;
    
    float lumaAverage = (1.0 / 12.0) * (2.0 * (lumaDownUp + lumaLeftRight) + lumaLeftCorners + lumaRightCorners);
    
    float subPixelOffset1 = clamp(abs(lumaAverage - lumaCenter) / lumaRange, 0.0, 1.0);
    float subPixelOffset2 = (-2.0 * subPixelOffset1 + 3.0) * subPixelOffset1 * subPixelOffset1;
    
    float subPixelOffsetFinal = subPixelOffset2 * subPixelOffset2 * SUBPIXEL_QUALITY;

    finalOffset = max(finalOffset, subPixelOffsetFinal);
    
    float2 finalUv = input.myUV;
    if (isHorizontal)
    {
        finalUv.y += finalOffset * stepLength;
    }
    else
    {
        finalUv.x += finalOffset * stepLength;
    }

    float3 finalColor = myFullscreenTexture1.Sample(myDefaultSampler, finalUv).rgb;
    result.myColor.rgb = finalColor;
    
    return result;
}