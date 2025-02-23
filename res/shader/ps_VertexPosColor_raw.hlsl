struct PIn {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 PS(PIn input) : SV_TARGET {
    return input.color;
}
