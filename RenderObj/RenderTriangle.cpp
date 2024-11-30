#include "RenderTriangle.h"
#include "GraphicsUtil.h"

RenderTriangle::RenderTriangle(std::string uuid, DirectX::XMFLOAT3* ps, DirectX::XMFLOAT4* cs) :
	RenderObj(uuid) {
    updatePos(ps);
    updateColor(cs);
}

RenderTriangle::~RenderTriangle() {}

bool RenderTriangle::init(ComPtr<ID3D11Device> dev) {
    if (!initDx(dev)) return false;
    renderReady = true;
    return true;
}

bool RenderTriangle::initDx(ComPtr<ID3D11Device> dev) {

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC; // write access access by CPU and GPU
    bd.ByteWidth = sizeof(VertexPosColor) * 3; // size is the VertexPosColor struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // allow CPU to write in buffer
    HRESULT hr = dev->CreateBuffer(&bd, NULL, vBuffer.GetAddressOf()); // create the buffer
    if (FAILED(hr)) {
        return false;
    }
    return true;
}

void RenderTriangle::render(ComPtr<ID3D11DeviceContext> devCon) const {
    if (!renderReady) return;

    VertexPosColor vertices[3];
    for (int i = 0; i < 3; ++i) {
        vertices[i] = { poses[i], colors[i] };
    }

    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    devCon->Map(vBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); // map the buffer
    memcpy(ms.pData, vertices, sizeof(vertices)); // copy the data
    devCon->Unmap(vBuffer.Get(), NULL); // unmap the buffer

    UINT stride = sizeof(VertexPosColor);
    UINT offset = 0;
    devCon->IASetVertexBuffers(0, 1, vBuffer.GetAddressOf(), &stride, &offset);
    devCon->Draw(3, 0);
}

void RenderTriangle::updatePos(DirectX::XMFLOAT3 *ps) {
    poses[0] = ps[0];
    poses[1] = ps[1];
    poses[2] = ps[2];
}

void RenderTriangle::updateColor(DirectX::XMFLOAT4 *cs) {
    colors[0] = cs[0];
    colors[1] = cs[1];
    colors[2] = cs[2];
}
