#include "RenderObj.h"
#include "GraphicsUtil.h"
#include "LogUtil.h"

RenderObjBase::RenderObjBase(std::string uuid, const WCHAR* vsHLSL, const WCHAR* psHLSL, D3D_PRIMITIVE_TOPOLOGY primitiveTopology) :
	vLayout(nullptr), vs(nullptr), ps(nullptr),
	vsHLSL(vsHLSL), psHLSL(psHLSL),
	uuid(uuid), primitiveTopology(primitiveTopology) {}

RenderObjBase::~RenderObjBase() {}

bool RenderObjBase::init(ComPtr<ID3D11Device> dev) {
	if (!initShader(dev)) return false;
	if (!initBuffer(dev)) return false;
	return true;
}

bool RenderObjBase::initShader(ComPtr<ID3D11Device> dev) {
	ComPtr<ID3DBlob> blob;

	// Compile and create vertex shader
	if (FAILED(CreateShaderFromFile(vsHLSL.c_str(), "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()))) {
		Logger::getInstance().error("[CreateShaderFromFile] Create vertex shader failed...");
		return false;
	}
	if (FAILED(dev->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, vs.GetAddressOf()))) {
		Logger::getInstance().error("[CreateVertexShader] Compile vertex shader failed...");
		return false;
	}

	// Init vertex layout
	if (!initLayout(dev, blob)) {
		Logger::getInstance().error("[initLayout] Init vertex shader layout failed...");
		return false;
	}
	
	// Compile and create pixel shader
	if (FAILED(CreateShaderFromFile(psHLSL.c_str(), "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()))) {
		Logger::getInstance().error("[CreateShaderFromFile] Create pixel shader failed...");
		return false;
	}
	if (FAILED(dev->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, ps.GetAddressOf()))) {
		Logger::getInstance().error("[CreateVertexShader] Compile pixel shader failed...");
		return false;
	}

	return true;
}

void RenderObjBase::render(ComPtr<ID3D11DeviceContext> devCon) const {
	devCon->VSSetShader(vs.Get(), nullptr, 0);
	devCon->PSSetShader(ps.Get(), nullptr, 0);
	devCon->IASetInputLayout(vLayout.Get());
	devCon->IASetPrimitiveTopology(primitiveTopology);
	doRender(devCon);
}

std::string RenderObjBase::getUUID() const {
	return uuid;
}
