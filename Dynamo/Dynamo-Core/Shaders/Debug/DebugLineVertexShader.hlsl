#include "DebugLineStructs.hlsli"

VertexOutput main(VertexInput aInput)
{
    VertexOutput output;

    // Remap from directx screen (-1 to 1) space to normalized 0 to 1 space
    float x = lerp(-1.f, 1.f, aInput.myPosition.x);
    float y = lerp(1.f, -1.f, aInput.myPosition.y);

    output.myPosition = float4(x, y, 0.f, 1.f);
    output.myColor = aInput.myColor;
    
    return output;
}