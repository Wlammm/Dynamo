#include "DebugLineStructs.hlsli"

PixelOutput main(VertexOutput aInput)
{
    PixelOutput output;
    
    output.myColor = aInput.myColor;
    
    return output;
}