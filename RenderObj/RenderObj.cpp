#include "RenderObj.h"
#include "GraphicsUtil.h"

RenderObj::RenderObj(std::string uuid, const WCHAR* vsHLSL, const WCHAR* psHLSL, D3D_PRIMITIVE_TOPOLOGY primitiveTopology) :
	vLayout(nullptr), vs(nullptr), ps(nullptr),
	vsHLSL(vsHLSL), psHLSL(psHLSL),
	uuid(uuid), primitiveTopology(primitiveTopology) {}

RenderObj::~RenderObj() {}

std::string RenderObj::getUUID() const {
	return uuid;
}

bool RenderObj::init(ComPtr<ID3D11Device> dev) {
	if (!initShader(dev)) return false;
	if (!initBuffer(dev)) return false;
	return true;
}

bool RenderObj::initShader(ComPtr<ID3D11Device> dev) {
	ComPtr<ID3DBlob> blob;

	// Compile and create vertex shader
	if (FAILED(CreateShaderFromFile(vsHLSL.c_str(), "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()))) {
		return false;
	}
	if (FAILED(dev->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, vs.GetAddressOf()))) {
		return false;
	}

	if (!initLayout(dev, blob)) return false;

	// Compile and create pixel shader
	if (FAILED(CreateShaderFromFile(psHLSL.c_str(), "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()))) {
		MessageBox(0, L"Compile pixel shader failed...", 0, 0);
		return false;
	}
	if (FAILED(dev->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, ps.GetAddressOf()))) {
		MessageBox(0, L"Create pixel shader failed...", 0, 0);
		return false;
	}

	return true;
}

void RenderObj::render(ComPtr<ID3D11DeviceContext> devCon) const {
	devCon->VSSetShader(vs.Get(), nullptr, 0);
	devCon->PSSetShader(ps.Get(), nullptr, 0);
	devCon->IASetInputLayout(vLayout.Get());
	devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	doRender(devCon);
}
