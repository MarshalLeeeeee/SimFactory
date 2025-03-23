#include "Mesh.h"

Mesh::Mesh() : loaded(false), primitiveTopology(D3D_PRIMITIVE_TOPOLOGY_UNDEFINED), indiceCnt(0) {}

bool Mesh::init(std::shared_ptr<MeshMeta> pMeshMeta, std::shared_ptr<VertexBuffer> pvb, ComPtr<ID3D11Device> dev, const D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSz, std::shared_ptr<TransformBuffer> pTransformData) {
    if (!pMeshMeta) return false;
    if (!pvb) return false;

    // topology
    primitiveTopology = static_cast<D3D_PRIMITIVE_TOPOLOGY>(pMeshMeta->getPrimitiveType());

    // vertex buffer
    pVertexBuffer = pvb;

    // index buffer
    indiceCnt = pMeshMeta->getIndiceCnt();
    std::shared_ptr<DWORD[]> indices = pMeshMeta->getIndices();
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_IMMUTABLE;
    bd.ByteWidth = sizeof(DWORD) * indiceCnt;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA initDataI;
    ZeroMemory(&initDataI, sizeof(initDataI));
    initDataI.pSysMem = indices.get();
    if (FAILED(dev->CreateBuffer(&bd, &initDataI, iBuffer.GetAddressOf()))) {
        return false;
    }

    // vertex material
    std::vector<size_t> vsszs;
    pVertexMaterial = std::make_shared<VertexMaterial>();
    if (!pVertexMaterial->init(dev, pMeshMeta, inputLayout, inputLayoutSz, vsszs)) {
        pVertexMaterial = nullptr;
        return false;
    }
    pVertexMaterialBuffer = std::make_shared<VMBPosColorRaw>(pTransformData);

    // pixel material
    std::vector<size_t> psszs;
    pPixelMaterial = std::make_shared<PixelMaterial>();
    if (!pPixelMaterial->init(dev, pMeshMeta, psszs)) {
        pPixelMaterial = nullptr;
        return false;
    }
    pPixelMaterialBuffer = std::make_shared<PMBPosColorRaw>();

    loaded = true;
    return true;
}

void Mesh::render(ComPtr<ID3D11DeviceContext> devCon) const {
    if (!loaded) return;
    pVertexMaterial->render(devCon);
    pPixelMaterial->render(devCon);
    pVertexMaterialBuffer->mapBuffer(devCon, pVertexMaterial);
    pPixelMaterialBuffer->mapBuffer(devCon, pPixelMaterial);
    devCon->IASetPrimitiveTopology(primitiveTopology);
    pVertexBuffer->render(devCon);
    devCon->IASetIndexBuffer(iBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    devCon->DrawIndexed(indiceCnt, 0, 0);
}