#pragma once

#ifndef __FILEUTIL_H__
#define __FILEUTIL_H__

// No project header is allowed
#include <windows.h>
#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include <ctime>
#include <unordered_map>
#include <functional>
#include <d3d11.h>
#include <DirectXMath.h>
#include "tinyxml2/tinyxml2.h"
// No project header is allowed

/* Return year-mon-day-hour-min-sec */
std::string getTimeStampStr();

/* Return year-mon-day-hour-min-sec */
std::string getTimeStampStr(const std::chrono::time_point<std::chrono::system_clock> now);

/* return if the directory exists */
bool directoryExists(const char* dir);

/* create the directory and return the result*/
bool createDirectory(const char* dir);

/* get vertex file name (relative to root) */
std::string getVertexFilepath(std::string fileName);

/* get model file name (relative to root) */
std::string getModelFilepath(std::string fileName);

/* get shader file name (relative to root) */
std::string getShaderFilepath(std::string fileName);

/* Mesh resource meta data */
class MeshMeta {
public:
    MeshMeta(std::string name, int primitiveType,
        std::string vertexShader, std::string pixelShader, std::string vertexLayout,
        std::vector<int>& indices);
    int getPrimitiveType() const;
    std::string getVertexShader() const;
    std::string getPixelShader() const;
    std::string getVertexLayout() const;
    std::string getName() const;
    std::shared_ptr<DWORD[]> getIndices() const;
    uint32_t getIndexCnt() const;
private:
    std::string name;
    int primitiveType;
    std::string vertexShader;
    std::string vertexLayout;
    std::string pixelShader;
    std::shared_ptr<DWORD[]> indices;
    uint32_t indexCnt;
};

/* Model resource meta data */
class ModelMetaBase {
public:
    void addMesh(std::shared_ptr<MeshMeta> pMeshMeta);
    std::string getVertexLayout() const;
    std::string getVertexFileName() const;
    const std::vector<std::shared_ptr<MeshMeta>>& getMeshMetaVec() const;

    virtual void* getData() const;
    virtual size_t getSize() const;
    virtual size_t getByteWidth() const;
    virtual const D3D11_INPUT_ELEMENT_DESC* getVertexLayoutDesc() const;
    virtual UINT getVertexLayoutDescSize() const;
protected:
    std::vector<std::shared_ptr<MeshMeta>> pMeshMetaVec;
    std::string vertexLayout;
    std::string vertexFileName;
};
template <typename T>
class ModelMeta : public ModelMetaBase {
public:
    void* getData() const { return vertexData.get(); }
    size_t getSize() const { return sizeof(T); }
    size_t getByteWidth() const { return sizeof(T) * vertexCnt; }
    const D3D11_INPUT_ELEMENT_DESC* getVertexLayoutDesc() const { return T::inputLayout; }
    UINT getVertexLayoutDescSize() const { return ARRAYSIZE(T::inputLayout); }
    void parse(tinyxml2::XMLElement* vertexDataNode, std::string layout, std::string vertexFileName) {
        vertexLayout = layout;
        vertexFileName = vertexFileName;
        std::vector<T> vertices;
        for (tinyxml2::XMLNode* vertex = vertexDataNode->FirstChildElement("Vertices")->FirstChild(); vertex; vertex = vertex->NextSibling()) {
            vertices.emplace_back(vertex);
        }
        vertexCnt = vertices.size();
        if (vertexCnt) {
            vertexData = std::make_unique<T[]>(vertexCnt);
            std::copy(vertices.begin(), vertices.end(), vertexData.get());
        }
    }
private:
    std::unique_ptr<T[]> vertexData;
    uint32_t vertexCnt;
};

/* load vertex resource sync */
std::shared_ptr<ModelMetaBase> loadVertexFromFile(std::string fileName);

/* load model resource async */
void loadModelFromFileAsync(std::string fileName, std::function<void(std::shared_ptr<ModelMetaBase>)> callback);

#endif
