#pragma once

#ifndef __GRAPHICS_UTIL_H__
#define __GRAPHICS_UTIL_H__

// No project header is allowed
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
// No project header is allowed

// compile shader
HRESULT CreateShaderFromFile(
	const WCHAR* hlslFileName,
	LPCSTR entryPoint,
	LPCSTR shaderModel,
	ID3DBlob** ppBlobOut
);

#endif
