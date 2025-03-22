cbuffer PixelBuffer : register(b0) {
    float4 data;
};

struct PIn {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 PS(PIn input) : SV_TARGET {
    return float4(input.color.rgb, data.r) * data.g;
}
