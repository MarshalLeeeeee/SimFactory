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

VertexMaterial::VertexMaterial(const std::string& vertexShaderFilename_) :
	Material(), vertexShaderFilename(vertexShaderFilename_) {}

bool VertexMaterial::initShader(ComPtr<ID3D11Device> dev, const D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSz) {
	ComPtr<ID3DBlob> blob;
	std::wstring vsHLSL = getWStringFromString(getShaderFilepath(vertexShaderFilename));

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

std::string VertexMaterial::getVertexShaderFilename() const {
	return vertexShaderFilename;
}

VertexMaterialBuffer::VertexMaterialBuffer(std::shared_ptr<TransformBuffer> data) :
	pTransformData(data) {
}

bool VertexMaterialBuffer::initMaterialBuffer(ComPtr<ID3D11Device> dev, std::shared_ptr<VertexMaterial> pVertexMaterial) {
	std::vector<size_t> vertexShaderBufferDataSizes;
	getBufferDataSizes(vertexShaderBufferDataSizes);
	if (!pVertexMaterial->initBuffers(dev, vertexShaderBufferDataSizes)) {
		Logger::getInstance().error("[VertexMaterialBuffer::initMetarialBuffer] Init vertex shader buffer failed...");
		return false;
	}
	return true;
}

void VertexMaterialBuffer::mapBuffer(ComPtr<ID3D11DeviceContext> devCon, std::shared_ptr<VertexMaterial> pVertexMaterial) const {
	mapConstantBuffer(devCon, pVertexMaterial->getBuffers()[0].Get(), pTransformData.get(), pVertexMaterial->getBufferDataSizes()[0]);
}

void VertexMaterialBuffer::updateBufferData(std::string& bufferName, const Any& any) {}

void VertexMaterialBuffer::getBufferDataSizes(std::vector<size_t>& vertexShaderBufferDataSizes) {
	vertexShaderBufferDataSizes.emplace_back(sizeof(TransformBuffer));
}

std::shared_ptr<VertexMaterial> VertexMaterialPool::getVertexMaterial(std::string vertexShaderFilename) const {
	auto itr = vertexMaterials.find(vertexShaderFilename);
	if (itr != vertexMaterials.end()) return itr->second;
	else return nullptr;
}

void VertexMaterialPool::addVertexMaterial(std::string vertexShaderFilename, std::shared_ptr<VertexMaterial> pVertexMaterial) {
	auto itr = vertexMaterials.find(vertexShaderFilename);
	if (itr != vertexMaterials.end()) return;
	vertexMaterials[vertexShaderFilename] = pVertexMaterial;
}
void VertexMaterialPool::deRefVertexMaterial(std::string vertexShaderFilename) {
	auto itr = vertexMaterials.find(vertexShaderFilename);
	if (itr == vertexMaterials.end()) return;
	if (itr->second.use_count() <= 2) {
		vertexMaterials.erase(itr);
	}
}

//////////////////////////////////////

PixelMaterial::PixelMaterial(const std::string& pixelShaderFilename_) :
	Material(), pixelShaderFilename(pixelShaderFilename_) {}

bool PixelMaterial::initShader(ComPtr<ID3D11Device> dev) {
	ComPtr<ID3DBlob> blob;
	std::wstring psHLSL = getWStringFromString(getShaderFilepath(pixelShaderFilename));

	if (FAILED(CreateShaderFromFile(psHLSL.c_str(), "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()))) {
		Logger::getInstance().error("[PixelMaterial::init] Create pixel shader failed...");
		return false;
	}
	if (FAILED(dev->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, pixelShader.GetAddressOf()))) {
		Logger::getInstance().error("[PixelMaterial::init] Compile pixel shader failed...");
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

std::string PixelMaterial::getPixelShaderFilename() const {
	return pixelShaderFilename;
}

PixelMaterialBuffer::PixelMaterialBuffer() {}

bool PixelMaterialBuffer::initMaterialBuffer(ComPtr<ID3D11Device> dev, std::shared_ptr<PixelMaterial> pPixelMaterial) {
	std::vector<size_t> pixelShaderBufferDataSizes;
	getBufferDataSizes(pixelShaderBufferDataSizes);
	if (!pPixelMaterial->initBuffers(dev, pixelShaderBufferDataSizes)) {
		Logger::getInstance().error("[PixelMaterialBuffer::initMaterialBuffer] Init pixel shader buffer failed...");
		return false;
	}
	return true;
}

void PixelMaterialBuffer::mapBuffer(ComPtr<ID3D11DeviceContext> devCon, std::shared_ptr<PixelMaterial> pPixelMaterial) const {}

void PixelMaterialBuffer::updateBufferData(std::string& bufferName, const Any& any) {}

void PixelMaterialBuffer::getBufferDataSizes(std::vector<size_t>& pixelShaderBufferDataSizes) {}

std::shared_ptr<PixelMaterial> PixelMaterialPool::getPixelMaterial(std::string pixelShaderFilename) const {
	auto itr = pixelMaterials.find(pixelShaderFilename);
	if (itr != pixelMaterials.end()) return itr->second;
	else return nullptr;
}

void PixelMaterialPool::addPixelMaterial(std::string pixelShaderFilename, std::shared_ptr<PixelMaterial> pPixelMaterial) {
	auto itr = pixelMaterials.find(pixelShaderFilename);
	if (itr != pixelMaterials.end()) return;
	pixelMaterials[pixelShaderFilename] = pPixelMaterial;
}

void PixelMaterialPool::deRefPixelMaterial(std::string pixelShaderFilename) {
	auto itr = pixelMaterials.find(pixelShaderFilename);
	if (itr == pixelMaterials.end()) return;
	if (itr->second.use_count() <= 2) {
		pixelMaterials.erase(itr);
	}
}

//////////////////////////////////////////////////

VMBPosColorRaw::VMBPosColorRaw(std::shared_ptr<TransformBuffer> data) :
	VertexMaterialBuffer(data) {}

PMBPosColorRaw::PMBPosColorRaw() {
	pPixelData = std::make_unique<PixelBuffer>();
	pPixelData->data = DirectX::XMFLOAT4(1.f, 1.f, 0.0f, 0.0f);
}

void PMBPosColorRaw::mapBuffer(ComPtr<ID3D11DeviceContext> devCon, std::shared_ptr<PixelMaterial> pPixelMaterial) const {
	mapConstantBuffer(devCon, pPixelMaterial->getBuffers()[0].Get(), pPixelData.get(), pPixelMaterial->getBufferDataSizes()[0]);
}

void PMBPosColorRaw::updateBufferData(std::string& bufferName, const Any& any) {
	if (bufferName == "pixelbuffer") {
		pPixelData->data = any.get<DirectX::XMFLOAT4>();
	}
}

void PMBPosColorRaw::getBufferDataSizes(std::vector<size_t>& pixelShaderBufferDataSizes) {
	PixelMaterialBuffer::getBufferDataSizes(pixelShaderBufferDataSizes);
	pixelShaderBufferDataSizes.emplace_back(sizeof(PixelBuffer));
}

//////////////////////////////////////////////////

std::unique_ptr<VertexMaterialBuffer> createVertexMaterialBuffer(const std::string& vertexShaderFilename, std::shared_ptr<TransformBuffer> pTransformData) {
	if (vertexShaderFilename == "vs_poscolor_raw") {
		return std::make_unique<VMBPosColorRaw>(pTransformData);
	}
	return nullptr;
}

std::unique_ptr<PixelMaterialBuffer> createPixelMaterialBuffer(const std::string& pixelShaderFilename) {
	if (pixelShaderFilename == "ps_poscolor_raw") {
		return std::make_unique<PMBPosColorRaw>();
	}
	return nullptr;
}