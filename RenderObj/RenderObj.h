#pragma once

#ifndef __REDNEROBJ_H__
#define __RENDEROBJ_H__

#include <Windows.h>
#include <wrl/client.h>
#include <DirectXMath.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <string>
#include <memory>

#include "TypeUtil.h"

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

class RenderObj {
public:
	RenderObj(std::string uuid, const WCHAR* vsHLSL, const WCHAR* psHLSL, D3D_PRIMITIVE_TOPOLOGY primitiveTopology);
	virtual ~RenderObj();

	bool init(ComPtr<ID3D11Device> dev);
	void render(ComPtr<ID3D11DeviceContext> devCon) const;

	std::string getUUID() const;

	virtual void updateField(uint32_t i, std::string fieldName, const Any& anyValue) = 0;

protected:
	bool initShader(ComPtr<ID3D11Device> dev);
	virtual bool initLayout(ComPtr<ID3D11Device> dev, ComPtr<ID3DBlob> blob) = 0;
	virtual bool initBuffer(ComPtr<ID3D11Device> dev) = 0;
	virtual void doRender(ComPtr<ID3D11DeviceContext> devCon) const = 0;

	ComPtr<ID3D11InputLayout> vLayout;
	ComPtr<ID3D11VertexShader> vs;
	ComPtr<ID3D11PixelShader> ps;

	std::string uuid;
	std::wstring vsHLSL;
	std::wstring psHLSL;
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology;
};

#endif
