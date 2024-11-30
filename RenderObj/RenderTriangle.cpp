#include "RenderTriangle.h"
#include "GraphicsUtil.h"

RenderTriangle::RenderTriangle(std::string uuid, double period) :
	RenderObj(uuid), period(10.0), c(1.0) {}

RenderTriangle::~RenderTriangle() {}

bool RenderTriangle::init(ComPtr<ID3D11Device> dev) {
    if (!initDx(dev)) return false;
    renderReady = true;
    return true;
}

bool RenderTriangle::initDx(ComPtr<ID3D11Device> dev) {

    /*VertexPosColor vertices[8];
    vertices[0] = { DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f), DirectX::XMFLOAT4(c, 0.0f, 0.0f, 1.0f) };
    vertices[1] = { DirectX::XMFLOAT3(0.45f, -0.5, 0.0f),  DirectX::XMFLOAT4(0.0f, c, 0.0f, 1.0f) };
    vertices[2] = { DirectX::XMFLOAT3(-0.45f, -0.5f, 0.0f),   DirectX::XMFLOAT4(0.0f, 0.0f, c, 1.0f) };*/
    
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

void RenderTriangle::update(double simTime, double frameTime) {
    updateC(simTime);
}

void RenderTriangle::render(ComPtr<ID3D11DeviceContext> devCon) {
    if (!renderReady) return;

    VertexPosColor vertices[3];
    vertices[0] = { DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f), DirectX::XMFLOAT4(c, 0.0f, 0.0f, 1.0f) };
    vertices[1] = { DirectX::XMFLOAT3(0.45f, -0.5, 0.0f),  DirectX::XMFLOAT4(0.0f, c, 0.0f, 1.0f) };
    vertices[2] = { DirectX::XMFLOAT3(-0.45f, -0.5f, 0.0f),   DirectX::XMFLOAT4(0.0f, 0.0f, c, 1.0f) };

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

void RenderTriangle::updateC(double simTime) {
    double diff = simTime - static_cast<int>(simTime / period) * period;
    double absDiff = std::abs(diff - period * 0.5);
    double result = absDiff / period * 2.0;
    c = static_cast<float>(result);
}
