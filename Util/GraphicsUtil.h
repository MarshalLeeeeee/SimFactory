#pragma once

#ifndef __GRAPHICS_UTIL_H__
#define __GRAPHICS_UTIL_H__

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

struct VertexPosColor {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
};

// compile shader
HRESULT CreateShaderFromFile(
	const WCHAR* hlslFileName,
	LPCSTR entryPoint,
	LPCSTR shaderModel,
	ID3DBlob** ppBlobOut
);

#endif
