#include "Model.h"
#include "VertexBuffer.h"
#include "ThreadUtil.h"
#include "MathUtil.h"
#include <functional>

Model::Model(ComPtr<ID3D11Device> dev, const char* fileName) : 
    awaken(false), loaded(false) {
    Dispatcher::getInstance().submit(0, [this, fileName, dev]{
        loadModelFromFileAsync(fileName, [this, dev](std::shared_ptr<ModelMetaBase> pModelMeta){
            doLoadFromModelMeta(dev, pModelMeta);
        });
    });
}

bool Model::getAwaken() const {
    return awaken;
}

bool Model::getLoaded() const {
    return loaded;
}

void Model::doLoadFromModelMeta(ComPtr<ID3D11Device> dev, std::shared_ptr<ModelMetaBase> pModelMeta) {
    awaken = true;
    if (!pModelMeta) {
        return;
    }
    
    // init vertex buffer
    // std::shared_ptr<VertexBuffer> pVertexBuffer = std::make_shared<VertexBuffer>();
    // if (!pVertexBuffer->init(dev, pModelMeta)) {
    //     pVertexBuffer = nullptr;
    //     return;
    // }
    
    std::string vertexFilename = pModelMeta->getVertexFilename();
    std::shared_ptr<VertexBuffer> pVertexBuffer = VertexBufferPool::getInstance().getVertexBuffer(vertexFilename);
    if (!pVertexBuffer) {
        pVertexBuffer = std::make_shared<VertexBuffer>(vertexFilename);
        if (pVertexBuffer->init(dev, pModelMeta)) {
            VertexBufferPool::getInstance().addVertexBuffer(vertexFilename, pVertexBuffer);
        }
        else {
            pVertexBuffer = nullptr;
        }
    }
    if (!pVertexBuffer) return;

    // load meshes
    pTransformData = std::make_shared<TransformBuffer>();
    pTransformData->mat = getTransformMatrix(0.f, 0.f, 0.f, 1.f, 1.f, 1.f);
    for (std::shared_ptr<MeshMeta> pMeshMeta : pModelMeta->getMeshMetas()) {
        std::shared_ptr<Mesh> pMesh = std::make_shared<Mesh>();
        if (pMesh->init(dev, pMeshMeta, pVertexBuffer, pModelMeta->getVertexLayoutDesc(), pModelMeta->getVertexLayoutDescSize(), pTransformData)) {
            pMeshes[pMeshMeta->getName()] = pMesh;
        }
        else {
            pMesh = nullptr;
        }
    }
    loaded = true;
}

void Model::render(ComPtr<ID3D11DeviceContext> devCon) const {
    for (auto itr = pMeshes.begin(); itr != pMeshes.end(); ++itr) {
        itr->second->render(devCon);
    }
}

void Model::updateTransformData(DirectX::XMMATRIX mat) {
    if (!pTransformData) return;
    pTransformData->mat = mat;
}

void Model::updateMeshVertexMaterialBuffer(std::string meshName, std::string bufferName, const Any& any) {
    auto itr = pMeshes.find(meshName);
    if (itr == pMeshes.end()) return;
    itr->second->updateVertexMaterialBuffer(bufferName, any);
}

void Model::updateMeshPixelMaterialBuffer(std::string meshName, std::string bufferName, const Any& any) {
    auto itr = pMeshes.find(meshName);
    if (itr == pMeshes.end()) return;
    itr->second->updatePixelMaterialBuffer(bufferName, any);
}