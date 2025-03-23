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

#include "GraphicsUtil.h"
#include "TypeUtil.h"

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

/*
 * Render obj is the primitive class for rendering
 * Holding vertex and pixel shaders
 * Holding vertex layout
 * Holding primitive topology
 */
class RenderObjBase {
public:
	RenderObjBase(std::string uuid, const WCHAR* vsHLSL, const WCHAR* psHLSL, D3D_PRIMITIVE_TOPOLOGY primitiveTopology);
	virtual ~RenderObjBase();

public:
    /* initialization : shader, buffer */
	bool init(ComPtr<ID3D11Device> dev);
protected:
    /* initialization of shader */
	bool initShader(ComPtr<ID3D11Device> dev);
    /* initialization of layout */
	virtual bool initLayout(ComPtr<ID3D11Device> dev, ComPtr<ID3DBlob> blob) = 0;
    /* initialization of buffer */
	virtual bool initBuffer(ComPtr<ID3D11Device> dev) = 0;
	ComPtr<ID3D11InputLayout> vLayout;
	ComPtr<ID3D11VertexShader> vs;
	ComPtr<ID3D11PixelShader> ps;
	std::wstring vsHLSL;
	std::wstring psHLSL;
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology;

public:
    /* prepare render, then draw */
	void render(ComPtr<ID3D11DeviceContext> devCon) const;
protected:
    /* draw */
	virtual void doRender(ComPtr<ID3D11DeviceContext> devCon) const = 0;

public:
    /* get uuid */
	std::string getUUID() const;
protected:
	std::string uuid;
};

/* 
 * Template cls inherited from RenderObjBase
 * Holding indices data
 * Sharing vertex data from RenderEntity
 */
template<typename T>
class RenderObj : public RenderObjBase {
public:
    RenderObj(std::string uuid, const WCHAR* vsHLSL, const WCHAR* psHLSL, D3D_PRIMITIVE_TOPOLOGY primitiveTopology, 
        std::shared_ptr<T[]> vData, uint32_t vCnt, DWORD* iData, uint32_t iCnt,
        std::shared_ptr<TransformBuffer> tfData, std::shared_ptr<PixelBuffer> pxData) :
        RenderObjBase(uuid, vsHLSL, psHLSL, primitiveTopology),
        vBuffer(nullptr), iBuffer(nullptr),
        vertices(vData), vertexCnt(vCnt), indexCnt(iCnt),
        pTfBufferData(tfData), pPxBufferData(pxData) {
			indices = std::make_unique<DWORD[]>(indexCnt);
			for (uint32_t i = 0; i < indexCnt; ++i) {
				indices[i] = iData[i];
			}
		}
	virtual ~RenderObj() {}

private:
    bool initLayout(ComPtr<ID3D11Device> dev, ComPtr<ID3DBlob> blob) {
        if (FAILED(dev->CreateInputLayout(T::inputLayout, ARRAYSIZE(T::inputLayout),
            blob->GetBufferPointer(), blob->GetBufferSize(), vLayout.GetAddressOf()))) {
            return false;
        }
        return true;
    }

    bool initBuffer(ComPtr<ID3D11Device> dev) {
        // vertex buffer
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.ByteWidth = sizeof(T) * vertexCnt;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA initDataV;
        ZeroMemory(&initDataV, sizeof(initDataV));
        initDataV.pSysMem = vertices.get();
        if (FAILED(dev->CreateBuffer(&bd, &initDataV, vBuffer.GetAddressOf()))) {
            return false;
        }
        // index buffer
        D3D11_BUFFER_DESC ibd;
        ZeroMemory(&ibd, sizeof(ibd));
        ibd.Usage = D3D11_USAGE_IMMUTABLE;
        ibd.ByteWidth = sizeof(DWORD) * indexCnt;
        ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        ibd.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA initDataI;
        ZeroMemory(&initDataI, sizeof(initDataI));
        initDataI.pSysMem = indices.get();
        if (FAILED(dev->CreateBuffer(&ibd, &initDataI, iBuffer.GetAddressOf()))) {
            return false;
        }
        // transform buffer
        if (!initConstantBuffer(dev, tfBuffer, sizeof(TransformBuffer))) {
            return false;
        }
        // pixel buffer
        if (!initConstantBuffer(dev, pxBuffer, sizeof(PixelBuffer))) {
            return false;
        }
        return true;
    }
    ComPtr<ID3D11Buffer> vBuffer;
    std::shared_ptr<T[]> vertices;
    uint32_t vertexCnt;

    ComPtr<ID3D11Buffer> iBuffer;
    std::unique_ptr<DWORD[]> indices;
    uint32_t indexCnt;

    ComPtr<ID3D11Buffer> tfBuffer; // transformBuffer for shader
    std::shared_ptr<TransformBuffer> pTfBufferData; // transformBuffer data

    ComPtr<ID3D11Buffer> pxBuffer; // pixelBuffer for shader
    std::shared_ptr<PixelBuffer> pPxBufferData; // pixelBuffer data

private:
	void doRender(ComPtr<ID3D11DeviceContext> devCon) const {
        // transform data
        mapConstantBuffer(devCon, tfBuffer.Get(), pTfBufferData.get(), sizeof(TransformBuffer));
        // pixel data
        mapConstantBuffer(devCon, pxBuffer.Get(), pPxBufferData.get(), sizeof(PixelBuffer));
        // draw
        UINT stride = sizeof(T);
        UINT offset = 0;
        devCon->IASetVertexBuffers(0, 1, vBuffer.GetAddressOf(), &stride, &offset);
        devCon->IASetIndexBuffer(iBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        devCon->VSSetConstantBuffers(0, 1, tfBuffer.GetAddressOf());
        devCon->PSSetConstantBuffers(0, 1, pxBuffer.GetAddressOf());
        devCon->DrawIndexed(indexCnt, 0, 0);
    }
};

#endif
