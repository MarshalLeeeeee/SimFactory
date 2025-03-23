#include "Model.h"
#include "ThreadUtil.h"
#include "MathUtil.h"
#include <functional>

template <typename T>
Model::Model(const char* fileName, ComPtr<ID3D11Device> dev) {
    Dispatcher::getInstance().submit(0, [this, fileName, dev]{
        loadModelFromFileAsync(fileName, [this, dev](std::shared_ptr<ModelMeta<T>> pModelMeta){
            onLoadedFromFile(pModelMeta, dev);
        });
    });
}

template <typename T>
void Model::onLoadedFromFile(std::shared_ptr<ModelMeta<T>> pModelMeta, ComPtr<ID3D11Device> dev) {
    if (!pModelMeta) return;

    // load vertex data
    std::string vertexFile = pModelMeta->getVertexFileName();
    // init vertex buffer
    bool vertexInited = false;
    std::shared_ptr<VertexBuffer> tmp = std::make_shared<VertexBuffer>();
    if (tmp->init(pModelMeta, dev)) {
        //pVertexBuffer = tmp;
        vertexInited = true;
    }
    if (!vertexInited) return;

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
        std::shared_ptr<Mesh> p = std::make_shared<Mesh>();
        if (p->init(pMeshMeta, tmp, dev, T::inputLayout, ARRAYSIZE(T::inputLayout), pTransformData)) {
            pMeshes[pMeshMeta->getName()] = p;
        }
    }
}
