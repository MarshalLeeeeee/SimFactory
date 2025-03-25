#include "Material.h"
#include "LogUtil.h"
#include "GraphicsUtil.h"

Material::Material() : bufferCnt(0) {}

const std::vector<ComPtr<ID3D11Buffer>>& Material::getBuffers() const {
	return buffers;
}

const std::vector<size_t>& Material::getBufferDataSizes() const {
	return bufferDataSizes;
}

uint32_t Material::getBufferCnt() const {
	return bufferCnt;
}

bool Material::initBuffers(ComPtr<ID3D11Device> dev, std::vector<size_t>& bufferDataSizes_) {
	bufferCnt = bufferDataSizes_.size();
	buffers.resize(bufferCnt);
	bufferDataSizes.resize(bufferCnt);
	for (uint32_t i = 0; i < bufferCnt; ++i) {
		if (!initConstantBuffer(dev, buffers[i], bufferDataSizes_[i])) {
			return false;
		}
		bufferDataSizes[i] = bufferDataSizes_[i];
	}
	return true;
}

////////////////////////////////////////

VertexMaterial::VertexMaterial() : Material() {}

bool VertexMaterial::init(ComPtr<ID3D11Device> dev, std::shared_ptr<MeshMeta> pMeshMeta, 
	const D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSz, std::vector<size_t>& bufferDataSizes_) {
	if (!pMeshMeta) return false;

	ComPtr<ID3DBlob> blob;
	vertexShaderFullname = pMeshMeta->getVertexShader();
	std::wstring vsHLSL = getWStringFromString(getShaderFilepath(vertexShaderFullname));

	if (FAILED(CreateShaderFromFile(vsHLSL.c_str(), "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()))) {
		Logger::getInstance().error("[VertexMaterial::init] Create vertex shader failed...");
		return false;
	}
	if (FAILED(dev->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, vertexShader.GetAddressOf()))) {
		Logger::getInstance().error("[VertexMaterial::init] Compile vertex shader failed...");
		return false;
	}

	if (FAILED(dev->CreateInputLayout(inputLayout, inputLayoutSz,
		blob->GetBufferPointer(), blob->GetBufferSize(), vertexLayout.GetAddressOf()))) {
		Logger::getInstance().error("[VertexMaterial::init] Init vertex shader layout failed...");
		return false;
	}
	
	if (!initBuffers(dev, bufferDataSizes_)) {
		Logger::getInstance().error("[VertexMaterial::init] Init vertex shader buffer failed...");
		return false;
	}

	return true;
}

void VertexMaterial::enableMaterial(ComPtr<ID3D11DeviceContext> devCon) const {
	devCon->VSSetShader(vertexShader.Get(), nullptr, 0);
	devCon->IASetInputLayout(vertexLayout.Get());
}

void VertexMaterial::enableBuffer(ComPtr<ID3D11DeviceContext> devCon) const {
	std::vector<ID3D11Buffer*> rawBuffers(bufferCnt);
	for (uint32_t i = 0; i < bufferCnt; ++i) {
		rawBuffers[i] = buffers[i].Get();
	}
	devCon->VSSetConstantBuffers(0, bufferCnt, rawBuffers.data());
}

VertexMaterialBuffer::VertexMaterialBuffer(std::shared_ptr<TransformBuffer> data) :
	pTransformData(data) {
}

void VertexMaterialBuffer::mapBuffer(ComPtr<ID3D11DeviceContext> devCon, std::shared_ptr<VertexMaterial> pVertexMaterial) const {
	mapConstantBuffer(devCon, pVertexMaterial->getBuffers()[0].Get(), pTransformData.get(), sizeof(TransformBuffer));
}

//////////////////////////////////////

PixelMaterial::PixelMaterial() : Material() {}

bool PixelMaterial::init(ComPtr<ID3D11Device> dev, std::shared_ptr<MeshMeta> pMeshMeta, 
	std::vector<size_t>& bufferDataSizes_) {
	if (!pMeshMeta) return false;

	ComPtr<ID3DBlob> blob;
	pixelShaderFullname = pMeshMeta->getPixelShader();
	std::wstring psHLSL = getWStringFromString(getShaderFilepath(pixelShaderFullname));

	if (FAILED(CreateShaderFromFile(psHLSL.c_str(), "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()))) {
		Logger::getInstance().error("[PixelMaterial::init] Create pixel shader failed...");
		return false;
	}
	if (FAILED(dev->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, pixelShader.GetAddressOf()))) {
		Logger::getInstance().error("[PixelMaterial::init] Compile pixel shader failed...");
		return false;
	}

	if (!initBuffers(dev, bufferDataSizes_)) {
		Logger::getInstance().error("[PixelMaterial::init] Init pixel shader buffer failed...");
		return false;
	}

	return true;
}

void PixelMaterial::enableMaterial(ComPtr<ID3D11DeviceContext> devCon) const {
	devCon->PSSetShader(pixelShader.Get(), nullptr, 0);
}

void PixelMaterial::enableBuffer(ComPtr<ID3D11DeviceContext> devCon) const {
	std::vector<ID3D11Buffer*> rawBuffers(bufferCnt);
	for (uint32_t i = 0; i < bufferCnt; ++i) {
		rawBuffers[i] = buffers[i].Get();
	}
	devCon->PSSetConstantBuffers(0, bufferCnt, rawBuffers.data());
}

PixelMaterialBuffer::PixelMaterialBuffer() {}

void PixelMaterialBuffer::mapBuffer(ComPtr<ID3D11DeviceContext> devCon, std::shared_ptr<PixelMaterial> pPixelMaterial) const {}

//////////////////////////////////////////////////

VMBPosColorRaw::VMBPosColorRaw(std::shared_ptr<TransformBuffer> data) :
	VertexMaterialBuffer(data) {}

PMBPosColorRaw::PMBPosColorRaw() {
	pPixelData = std::make_unique<PixelBuffer>();
	pPixelData->data = DirectX::XMFLOAT4(1.f, 1.f, 0.0f, 0.0f);
}

void PMBPosColorRaw::mapBuffer(ComPtr<ID3D11DeviceContext> devCon, std::shared_ptr<PixelMaterial> pPixelMaterial) const {
	mapConstantBuffer(devCon, pPixelMaterial->getBuffers()[0].Get(), pPixelData.get(), sizeof(PixelBuffer));
}
