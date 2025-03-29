#include "GraphicsUtil.h"

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
