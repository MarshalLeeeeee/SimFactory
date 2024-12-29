#pragma once

#ifndef __GRAPHICS_UTIL_H__
#define __GRAPHICS_UTIL_H__

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

// compile shader
HRESULT CreateShaderFromFile(
	const WCHAR* hlslFileName,
	LPCSTR entryPoint,
	LPCSTR shaderModel,
	ID3DBlob** ppBlobOut
);

#endif
