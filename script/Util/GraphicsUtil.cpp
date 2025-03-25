#include "GraphicsUtil.h"
#include <type_traits>
#include <string>

///////////////
// compile shader
HRESULT CreateShaderFromFile(
	const WCHAR* hlslFileName,
	LPCSTR entryPoint,
	LPCSTR shaderModel,
	ID3DBlob** ppBlobOut
) {
	HRESULT hr = D3DCompileFromFile(
		hlslFileName,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint,
		shaderModel,
		D3DCOMPILE_ENABLE_STRICTNESS,
		0,
		ppBlobOut,
		nullptr
	);
	return hr;
}

bool initConstantBuffer(ComPtr<ID3D11Device> dev, ComPtr<ID3D11Buffer>& buffer, size_t bufferDataSz) {
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = bufferDataSz;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (FAILED(dev->CreateBuffer(&bd, NULL, buffer.GetAddressOf()))) {
		return false;
	}
	return true;
}

void mapConstantBuffer(ComPtr<ID3D11DeviceContext>& devCon, ID3D11Resource* buffer, void* data, size_t dataSize) {
	D3D11_MAPPED_SUBRESOURCE ms;
	devCon->Map(buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, data, dataSize);
	devCon->Unmap(buffer, NULL);
}

/////////////////////////////////

VertexBuffer::~VertexBuffer() {
	if (vertices) {
		::operator delete(vertices);
	}
}

bool VertexBuffer::init(ComPtr<ID3D11Device> dev, std::shared_ptr<ModelMetaBase> pModelMeta) {
	if (!pModelMeta) return false;

	// vertices data
	vertices = ::operator new(pModelMeta->getByteWidth());
	if (!vertices) return false;
	memcpy(vertices, pModelMeta->getData(), pModelMeta->getByteWidth());

	// vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = pModelMeta->getByteWidth();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertices;
	if (FAILED(dev->CreateBuffer(&bd, &initData, vBuffer.GetAddressOf()))) {
		return false;
	}
	stride = pModelMeta->getSize();
	return true;
}

ComPtr<ID3D11Buffer> VertexBuffer::getVBuffer() const {
	return vBuffer;
}

void VertexBuffer::enableVertexBuffer(ComPtr<ID3D11DeviceContext> devCon) const {
	UINT offset = 0;
	devCon->IASetVertexBuffers(0, 1, vBuffer.GetAddressOf(), &stride, &offset);
}
