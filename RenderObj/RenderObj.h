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

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

class RenderObj {
public:
	RenderObj(std::string uuid);
	virtual ~RenderObj();

	virtual bool init(ComPtr<ID3D11Device> dev) = 0;
	virtual void render(ComPtr<ID3D11DeviceContext> devCon) const = 0;

	virtual void updatePos(DirectX::XMFLOAT3* ps) = 0;
	virtual void updateColor(DirectX::XMFLOAT4* cs) = 0;

	std::string getUUID() const;

protected:
	ComPtr<ID3D11Buffer> vBuffer;
	std::string uuid;
	bool renderReady;
};

#endif
