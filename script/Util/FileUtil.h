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

/* get shader file name (relative to root) */
std::string getShaderConfFilepath(std::string fileName);

/* Mesh resource meta data */
class MeshMeta {
public:
    MeshMeta(std::string name, int primitiveType,
        std::string vertexShaderName, std::string pixelShaderName, std::string vertexLayoutName,
        std::vector<int>& indices);
    /* get mesh name (defined in model xml) */
    std::string getName() const;
    /* get primitive type */
    int getPrimitiveType() const;
    /* get vertex shader file name */
    const std::string& getVertexShaderFilename() const;
    /* get pixel shader file name */
    const std::string& getPixelShaderFilename() const;
    /* get indices data */
    std::shared_ptr<DWORD[]> getIndices() const;
    /* get index cnt */
    uint32_t getIndexCnt() const;
private:
    /* mesh name */
    std::string name;
    /* primitive type */
    int primitiveType;
    /* vertex shader file name */
    std::string vertexShaderFilename;
    /* pixel shader file name */
    std::string pixelShaderFilename;
    /* indices data */
    std::shared_ptr<DWORD[]> indices;
    /* index cnt */
    uint32_t indexCnt;
};

/* Model resource meta data */
class ModelMetaBase {
public:
    /* add mesh meta */
    void addMeshMeta(std::shared_ptr<MeshMeta> pMeshMeta);
    /* get mesh metas */
    const std::vector<std::shared_ptr<MeshMeta>>& getMeshMetas() const;
    /* get vertex layout name */
    std::string getVertexLayoutName() const;
    /* get vertex filename */
    std::string getVertexFilename() const;

    /* get vertex data */
    virtual void* getVertexData() const;
    /* get size of vertex data type */
    virtual size_t getVertexDataTypeSize() const;
    /* get byte width of vertex data */
    virtual size_t getVertexDataByteWidth() const;
    /* get vertex layout desc */
    virtual const D3D11_INPUT_ELEMENT_DESC* getVertexLayoutDesc() const;
    /* get length of vertex layout desc */
    virtual UINT getVertexLayoutDescSize() const;
protected:
    /* mesh metas */
    std::vector<std::shared_ptr<MeshMeta>> pMeshMetas;
    /* vertex layout name */
    std::string vertexLayoutName;
    /* vertex file name */
    std::string vertexFilename;
};
template <typename T>
class ModelMeta : public ModelMetaBase {
public:
    /* get vertex data */
    void* getVertexData() const { 
        if (!vertexData) return nullptr;
        return vertexData.get(); 
    }
    /* get size of vertex data type */
    size_t getVertexDataTypeSize() const { return sizeof(T); }
    /* get byte width of vertex data */
    size_t getVertexDataByteWidth() const { return sizeof(T) * vertexCnt; }
    /* get vertex layout desc */
    const D3D11_INPUT_ELEMENT_DESC* getVertexLayoutDesc() const { return T::inputLayout; }
    /* get length of vertex layout desc */
    UINT getVertexLayoutDescSize() const { return ARRAYSIZE(T::inputLayout); }
    /* initialize from xml node */
    void parse(tinyxml2::XMLElement* vertexDataNode, std::string vertexLayoutName_, std::string vertexFilename_) {
        vertexLayoutName = vertexLayoutName_;
        vertexFilename = vertexFilename_;
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
    /* vertex data */
    std::unique_ptr<T[]> vertexData;
    /* vertex cnt */
    uint32_t vertexCnt;
};

/* load vertex resource sync */
std::shared_ptr<ModelMetaBase> loadVertexFromFile(std::string fileName);

/* load model resource async */
void loadModelFromFileAsync(std::string fileName, std::function<void(std::shared_ptr<ModelMetaBase>)> callback);

#endif
