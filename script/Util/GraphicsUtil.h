#pragma once

#ifndef __GRAPHICS_UTIL_H__
#define __GRAPHICS_UTIL_H__

// No project header is allowed
#include <Windows.h>
#include <wrl/client.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#include <memory>
#include "FileUtil.h"
// No project header is allowed

// compile shader
HRESULT CreateShaderFromFile(
	const WCHAR* hlslFileName,
	LPCSTR entryPoint,
	LPCSTR shaderModel,
	ID3DBlob** ppBlobOut
);

/* init constant buffer */
bool initConstantBuffer(ComPtr<ID3D11Device> dev, ComPtr<ID3D11Buffer>& buffer, size_t bufferDataSz);

/* map constant buffer */
void mapConstantBuffer(ComPtr<ID3D11DeviceContext>& devCon, ID3D11Resource* buffer, void* data, size_t dataSize);

/* Vertex buffer */
class VertexBuffer {
public:
	virtual ~VertexBuffer();
	bool init(ComPtr<ID3D11Device> dev, std::shared_ptr<ModelMetaBase> pModelMeta);
	ComPtr<ID3D11Buffer> getVBuffer() const;
	void enableVertexBuffer(ComPtr<ID3D11DeviceContext> devCon) const;
private:
	ComPtr<ID3D11Buffer> vBuffer;
	void* vertices;
	UINT stride;
};

#endif
