cbuffer TransformBuffer : register(b0) {
    float4x4 transformMat;
};

struct VOut {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VOut VS(float4 position : POSITION, float4 color : COLOR) {
    VOut output;

    output.position = mul(transformMat, position);
    output.color = color;

    return output;
}
