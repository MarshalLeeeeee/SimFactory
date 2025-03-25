#include "Model.h"
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

void Model::doLoadFromModelMeta(ComPtr<ID3D11Device> dev, std::shared_ptr<ModelMetaBase> pModelMeta) {
    awaken = true;
    if (!pModelMeta) {
        loaded = false;
        return;
    }
    
    // init vertex buffer
    std::shared_ptr<VertexBuffer> tmp = std::make_shared<VertexBuffer>();
    if (!tmp->init(dev, pModelMeta)) {
        tmp = nullptr;
        loaded = false;
        return;
    }
    
    // std::string vertexFile = pModelMeta->getVertexFileName();
    // std::shared_ptr<VertexBuffer> tmp = VertexBufferManager::getInstance().getRef(vertexFile);
    // if (tmp) {
    //     pVertexBuffer = tmp;
    // }
    // else {
    //     tmp = std::make_shared<VertexBuffer>();
    //     if (tmp->init(pModelMeta)) {
    //         pVertexBuffer = tmp;
    //         VertexBufferManager::getInstance().incRef(vertexFile, pVertexBuffer);
    //     }
    // }

    // load meshes
    pTransformData = std::make_shared<TransformBuffer>();
    pTransformData->mat = getTransformMatrix(0.f, 0.f, 0.f, 1.f, 1.f, 1.f);
    for (auto pMeshMeta : pModelMeta->getMeshMetaVec()) {
        std::shared_ptr<Mesh> pMesh = std::make_shared<Mesh>();
        if (pMesh->init(dev, pMeshMeta, tmp, pModelMeta->getVertexLayoutDesc(), pModelMeta->getVertexLayoutDescSize(), pTransformData)) {
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
