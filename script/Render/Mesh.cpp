#include "Mesh.h"
#include "TypeUtil.h"

Mesh::Mesh() : loaded(false), primitiveTopology(D3D_PRIMITIVE_TOPOLOGY_UNDEFINED), indexCnt(0) {}

bool Mesh::init(ComPtr<ID3D11Device> dev, std::shared_ptr<MeshMeta> pMeshMeta, std::shared_ptr<VertexBuffer> pVertexBuffer_, 
    const D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSize, std::shared_ptr<TransformBuffer> pTransformData) {
    if (!pMeshMeta) return false;
    if (!pVertexBuffer_) return false;

    // topology
    primitiveTopology = static_cast<D3D_PRIMITIVE_TOPOLOGY>(pMeshMeta->getPrimitiveType());

    // vertex buffer
    pVertexBuffer = pVertexBuffer_;

    // index buffer
    indexCnt = pMeshMeta->getIndexCnt();
    indices = std::make_unique<DWORD[]>(indexCnt);
    std::shared_ptr<DWORD[]> tmp = pMeshMeta->getIndices();
    std::copy(tmp.get(), tmp.get() + indexCnt, indices.get());
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_IMMUTABLE;
    bd.ByteWidth = sizeof(DWORD) * indexCnt;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA initDataI;
    ZeroMemory(&initDataI, sizeof(initDataI));
    initDataI.pSysMem = indices.get();
    if (FAILED(dev->CreateBuffer(&bd, &initDataI, indexBuffer.GetAddressOf()))) {
        return false;
    }

    // vertex material
    std::vector<size_t> vertexShaderBufferDataSizes;
    vertexShaderBufferDataSizes.emplace_back(sizeof(TransformBuffer));
    pVertexMaterial = std::make_shared<VertexMaterial>();
    if (!pVertexMaterial->init(dev, pMeshMeta, inputLayout, inputLayoutSize, vertexShaderBufferDataSizes)) {
        pVertexMaterial = nullptr;
        return false;
    }
    pVertexMaterialBuffer = std::make_shared<VMBPosColorRaw>(pTransformData);

    // pixel material
    std::vector<size_t> pixelShaderBufferDataSizes;
    pixelShaderBufferDataSizes.emplace_back(sizeof(PixelBuffer));
    pPixelMaterial = std::make_shared<PixelMaterial>();
    if (!pPixelMaterial->init(dev, pMeshMeta, pixelShaderBufferDataSizes)) {
        pPixelMaterial = nullptr;
        return false;
    }
    pPixelMaterialBuffer = std::make_shared<PMBPosColorRaw>();

    // everything is okay
    loaded = true;
    return true;
}

void Mesh::render(ComPtr<ID3D11DeviceContext> devCon) const {
    if (!loaded) return;
    pVertexMaterial->enableMaterial(devCon);
    pPixelMaterial->enableMaterial(devCon);
    devCon->IASetPrimitiveTopology(primitiveTopology);
    pVertexMaterialBuffer->mapBuffer(devCon, pVertexMaterial);
    pPixelMaterialBuffer->mapBuffer(devCon, pPixelMaterial);
    pVertexBuffer->enableVertexBuffer(devCon);
    devCon->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    pVertexMaterial->enableBuffer(devCon);
    pPixelMaterial->enableBuffer(devCon);
    devCon->DrawIndexed(indexCnt, 0, 0);
}