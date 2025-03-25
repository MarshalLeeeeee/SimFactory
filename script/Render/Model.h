#pragma once

#ifndef __MODEL_H__
#define __MODEL_H__

#include <memory>
#include <string>
#include <unordered_map>
#include "Mesh.h"
#include "FileUtil.h"
#include "TypeUtil.h"
#include "GraphicsUtil.h"

class Model {
public:
    Model(ComPtr<ID3D11Device> dev, const char* fileName);
private:
    /* do load model given ModelMeta<T> */
    void doLoadFromModelMeta(ComPtr<ID3D11Device> dev, std::shared_ptr<ModelMetaBase> pModelMeta);

public:
    void render(ComPtr<ID3D11DeviceContext> devCon) const;
private:
    std::unordered_map<std::string, std::shared_ptr<Mesh>> pMeshes;
    std::shared_ptr<TransformBuffer> pTransformData;
    bool awaken;
    bool loaded;
};

#endif
