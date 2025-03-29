#include "Mesh.h"
#include "TypeUtil.h"

Mesh::Mesh() : loaded(false), primitiveTopology(D3D_PRIMITIVE_TOPOLOGY_UNDEFINED) {}

Mesh::~Mesh() {
    if (pVertexBuffer) {
        VertexBufferPool::getInstance().deRefVertexBuffer(pVertexBuffer->getVertexFilename());
    }
    if (pIndexBuffer) {
        IndexBufferPool::getInstance().deRefIndexBuffer(pIndexBuffer->getIndexAssetName());
    }
    if (pVertexMaterial) {
        VertexMaterialPool::getInstance().deRefVertexMaterial(pVertexMaterial->getVertexShaderFilename());
    }
    if (pPixelMaterial) {
        PixelMaterialPool::getInstance().deRefPixelMaterial(pPixelMaterial->getPixelShaderFilename());
    }
}

bool Mesh::init(ComPtr<ID3D11Device> dev, std::shared_ptr<MeshMeta> pMeshMeta, std::shared_ptr<VertexBuffer> pVertexBuffer_, 
    const D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSize, std::shared_ptr<TransformBuffer> pTransformData) {
    if (!pMeshMeta) return false;
    if (!pVertexBuffer_) return false;

    // topology
    primitiveTopology = static_cast<D3D_PRIMITIVE_TOPOLOGY>(pMeshMeta->getPrimitiveType());

    // vertex buffer
    pVertexBuffer = pVertexBuffer_;

    // index buffer
    std::string indexAssetName = pMeshMeta->getName();
    pIndexBuffer = IndexBufferPool::getInstance().getIndexBuffer(indexAssetName);
    if (!pIndexBuffer) {
        pIndexBuffer = std::make_shared<IndexBuffer>(indexAssetName);
        if (pIndexBuffer->init(dev, pMeshMeta->getIndexCnt(), pMeshMeta->getIndices())) {
            IndexBufferPool::getInstance().addIndexBuffer(indexAssetName, pIndexBuffer);
        }
        else {
            pIndexBuffer = nullptr;
        }
    }
    if (!pIndexBuffer) return false;

    // vertex material
    std::string vertexShaderFilename = pMeshMeta->getVertexShaderFilename();
    pVertexMaterial = VertexMaterialPool::getInstance().getVertexMaterial(vertexShaderFilename);
    if (!pVertexMaterial) {
        pVertexMaterial = std::make_shared<VertexMaterial>(vertexShaderFilename);
        if (pVertexMaterial->initShader(dev, inputLayout, inputLayoutSize)) {
            VertexMaterialPool::getInstance().addVertexMaterial(vertexShaderFilename, pVertexMaterial);
        }
        else {
            pVertexMaterial = nullptr;
        }
    }
    if (!pVertexMaterial) return false;
    pVertexMaterialBuffer = std::move(createVertexMaterialBuffer(pMeshMeta->getVertexShaderFilename(), pTransformData));
    if (!pVertexMaterialBuffer) return false;
    if (!pVertexMaterialBuffer->initMaterialBuffer(dev, pVertexMaterial)) {
        pVertexMaterial = nullptr;
        pVertexMaterialBuffer = nullptr;
        return false;
    }

    // pixel material
    std::string pixelShaderFilename = pMeshMeta->getPixelShaderFilename();
    pPixelMaterial = PixelMaterialPool::getInstance().getPixelMaterial(pixelShaderFilename);
    if (!pPixelMaterial) {
        pPixelMaterial = std::make_shared<PixelMaterial>(pixelShaderFilename);
        if (pPixelMaterial->initShader(dev)) {
            PixelMaterialPool::getInstance().addPixelMaterial(pixelShaderFilename, pPixelMaterial);
        }
        else {
            pPixelMaterial = nullptr;
        }
    }
    if (!pPixelMaterial) return false;
    pPixelMaterialBuffer = std::move(createPixelMaterialBuffer(pMeshMeta->getPixelShaderFilename()));
    if (!pPixelMaterialBuffer) return false;
    if (!pPixelMaterialBuffer->initMaterialBuffer(dev, pPixelMaterial)) {
        pPixelMaterial = nullptr;
        pPixelMaterialBuffer = nullptr;
        return false;
    }

    // everything is okay
    loaded = true;
    return true;
}

void Mesh::render(ComPtr<ID3D11DeviceContext> devCon) const {
    if (!loaded) return;
    // enable material
    pVertexMaterial->enableMaterial(devCon);
    pPixelMaterial->enableMaterial(devCon);
    // set primitive topology
    devCon->IASetPrimitiveTopology(primitiveTopology);
    // map material buffer data
    pVertexMaterialBuffer->mapBuffer(devCon, pVertexMaterial);
    pPixelMaterialBuffer->mapBuffer(devCon, pPixelMaterial);
    // enable vertex buffer and index buffer
    pVertexBuffer->enableVertexBuffer(devCon);
    pIndexBuffer->enableIndexBuffer(devCon);
    // enable material buffer
    pVertexMaterial->enableBuffer(devCon);
    pPixelMaterial->enableBuffer(devCon);
    // draw
    pIndexBuffer->drawIndex(devCon);
}

void Mesh::updateVertexMaterialBuffer(std::string& bufferName, const Any& any) {
    if (!pVertexMaterialBuffer) return;
    pVertexMaterialBuffer->updateBufferData(bufferName, any);
}

void Mesh::updatePixelMaterialBuffer(std::string& bufferName, const Any& any) {
    if (!pPixelMaterialBuffer) return;
    pPixelMaterialBuffer->updateBufferData(bufferName, any);
}