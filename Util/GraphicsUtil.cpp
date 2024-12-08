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
