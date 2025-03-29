#pragma once

#ifndef __MODEL_H__
#define __MODEL_H__

#include <memory>
#include <string>
#include <unordered_map>
#include "Mesh.h"
#include "FileUtil.h"
#include "TypeUtil.h"

class Model {
public:
    Model(ComPtr<ID3D11Device> dev, const char* fileName);
    /* get awaken */
    bool getAwaken() const;
    /* get loaded */
    bool getLoaded() const;
private:
    /* do load model given ModelMeta<T> (in main thread) */
    void doLoadFromModelMeta(ComPtr<ID3D11Device> dev, std::shared_ptr<ModelMetaBase> pModelMeta);
    /* if ModelMeta has been parsed and tried to load */
    bool awaken;
    /* if model is successfully loaded, i.e. ready to be rendered */
    bool loaded;

public:
    /* render model */
    void render(ComPtr<ID3D11DeviceContext> devCon) const;
    /* update transformation data */
    void updateTransformData(DirectX::XMMATRIX mat);
    /* update mesh vertex shader buffer */
    void updateMeshVertexMaterialBuffer(std::string meshName, std::string bufferName, const Any& any);
    /* update mesh vertex shader buffer */
    void updateMeshPixelMaterialBuffer(std::string meshName, std::string bufferName, const Any& any);
private:
    /* mesh_name --> mesh */
    std::unordered_map<std::string, std::shared_ptr<Mesh>> pMeshes;
    /* transformation of the whole model */
    std::shared_ptr<TransformBuffer> pTransformData;
};

#endif
