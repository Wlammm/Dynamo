#include "../ShaderStructs.hlsli"

struct Output
{
    uint ID : SV_TARGET;
};

cbuffer SelectionBuffer : register(b8)
{
    int myGameObjectID = 0;
    int3 padding;
}

Output main(VertexOutput input)
{
    Output output;
    output.ID = myGameObjectID;
    return output;
}