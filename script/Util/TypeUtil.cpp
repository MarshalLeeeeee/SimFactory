#include "TypeUtil.h"

const D3D11_INPUT_ELEMENT_DESC VertexPosColor::inputLayout[2] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

void VertexPosColor::updateField(const std::string fieldName, const Any& anyValue) {
	if (fieldName == "pos") {
        pos = anyValue.get<DirectX::XMFLOAT3>();
	}
	else if (fieldName == "color") {
        color = anyValue.get<DirectX::XMFLOAT4>();
	}
	else {}
}
