#pragma once

#ifndef __REDNERGEOMETRY_H__
#define __RENDERGEOMETRY_H__

#include "RenderObj.h"

template<typename T>
class RenderGeometry : public RenderObj {
public:
    RenderGeometry(std::string uuid, const WCHAR* vsHLSL, const WCHAR* psHLSL, D3D_PRIMITIVE_TOPOLOGY primitiveTopology, 
        T* vData, uint32_t vCnt, DWORD* iData, uint32_t iCnt) :
        RenderObj(uuid, vsHLSL, psHLSL, primitiveTopology),
        vBuffer(nullptr), iBuffer(nullptr),
        vertexCnt(vCnt), indexCnt(iCnt) {
        vertices = std::make_unique<T[]>(vCnt);
        for (int i = 0; i < vCnt; ++i) {
            vertices[i] = vData[i];
        }
        indices = std::make_unique<DWORD[]>(iCnt);
        for (int i = 0; i < iCnt; ++i) {
            indices[i] = iData[i];
        }
    }
	virtual ~RenderGeometry() {}

    void updateField(uint32_t i, std::string fieldName, const Any& anyValue) {
        if (i >= vertexCnt) return;
        vertices[i].updateField(fieldName, anyValue);
    }

protected:
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
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(T) * vertexCnt;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        HRESULT hr = dev->CreateBuffer(&bd, NULL, vBuffer.GetAddressOf()); // create the buffer
        if (FAILED(hr)) {
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

        return true;
    }

	void doRender(ComPtr<ID3D11DeviceContext> devCon) const {

        D3D11_MAPPED_SUBRESOURCE ms;
        devCon->Map(vBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); // map the buffer
        memcpy(ms.pData, vertices.get(), sizeof(T) * vertexCnt); // copy the data
        devCon->Unmap(vBuffer.Get(), NULL); // unmap the buffer

        UINT stride = sizeof(T);
        UINT offset = 0;
        devCon->IASetVertexBuffers(0, 1, vBuffer.GetAddressOf(), &stride, &offset);
        devCon->IASetIndexBuffer(iBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        devCon->DrawIndexed(indexCnt, 0, 0);
    }

    ComPtr<ID3D11Buffer> vBuffer;
    std::unique_ptr<T[]> vertices;
    uint32_t vertexCnt;

    ComPtr<ID3D11Buffer> iBuffer;
    std::unique_ptr<DWORD[]> indices;
    uint32_t indexCnt;
};

#endif
