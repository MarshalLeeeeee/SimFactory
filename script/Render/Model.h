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
    template <typename T>
    Model(const char* fileName, ComPtr<ID3D11Device> dev);
private:
    template <typename T>
    void onLoadedFromFile(std::shared_ptr<ModelMeta<T>> pModelMeta, ComPtr<ID3D11Device> dev);
private:
    std::unordered_map<std::string, std::shared_ptr<Mesh>> pMeshes;
    std::shared_ptr<TransformBuffer> pTransformData;
};

#endif
