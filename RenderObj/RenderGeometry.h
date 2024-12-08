#pragma once

#ifndef __REDNERGEOMETRY_H__
#define __RENDERGEOMETRY_H__

#include "RenderObj.h"

template<typename T>
class RenderGeometry : public RenderObj {
public:
    RenderGeometry(std::string uuid, const WCHAR* vsHLSL, const WCHAR* psHLSL, D3D_PRIMITIVE_TOPOLOGY primitiveTopology, T* data, uint32_t cnt) :
        RenderObj(uuid, vsHLSL, psHLSL, primitiveTopology),
        vBuffer(nullptr),
        vertexCnt(cnt) {
        vertices = std::make_unique<T[]>(cnt);
        for (int i = 0; i < cnt; ++i) {
            vertices[i] = data[i];
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
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(T) * 3;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        HRESULT hr = dev->CreateBuffer(&bd, NULL, vBuffer.GetAddressOf()); // create the buffer
        if (FAILED(hr)) {
            return false;
        }
        return true;
    }
	void doRender(ComPtr<ID3D11DeviceContext> devCon) const {

        D3D11_MAPPED_SUBRESOURCE ms;
        devCon->Map(vBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); // map the buffer
        memcpy(ms.pData, vertices.get(), vertexCnt * sizeof(T)); // copy the data
        devCon->Unmap(vBuffer.Get(), NULL); // unmap the buffer

        UINT stride = sizeof(T);
        UINT offset = 0;
        devCon->IASetVertexBuffers(0, 1, vBuffer.GetAddressOf(), &stride, &offset);
        devCon->Draw(vertexCnt, 0);
    }

    ComPtr<ID3D11Buffer> vBuffer;
    uint32_t vertexCnt;
    std::unique_ptr<T[]> vertices;
};

#endif
