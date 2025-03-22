#pragma once

#ifndef __FILEUTIL_H__
#define __FILEUTIL_H__

// No project header is allowed
#include <windows.h>
#include <memory>
#include <string>
#include <chrono>
#include <ctime>
#include <unordered_map>
#include <functional>
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

/* get vertex file name */
std::string getVertexFilepath(std::string fileName);

/* get model file name */
std::string getModelFilepath(std::string fileName);

/* Mesh resource meta data */
class MeshMeta {
public:
    MeshMeta(std::string, int, std::string, std::string, std::vector<int>&);
    void setVertexLayout(std::string);
    std::string getName() const;
private:
    std::string name;
    int primitiveType;
    std::string vertexShader;
    std::string vertexLayout;
    std::string pixelShader;
    std::shared_ptr<DWORD[]> indices;
    uint32_t indiceCnt;
};

/* Model resource meta data */
class ModelMetaBase {
public:
    void addMesh(std::shared_ptr<MeshMeta>);
    std::string getVertexLayout() const;
protected:
    std::vector<std::shared_ptr<MeshMeta>> pMeshMetaVec;
    std::string vertexLayout;
};
template <typename T>
class ModelMeta : public ModelMetaBase {
public:
    std::shared_ptr<T[]> getData() { return vertexData; }
    size_t getSize() { return sizeof(T); }
    size_t getByteWidth() { return sizeof(T) * vertexCnt; }
    void parse(tinyxml2::XMLElement* vertexDataNode, std::string layout) {
        vertexLayout = layout;
        std::vector<T> vertices;
        for (tinyxml2::XMLNode* vertex = vertexDataNode->FirstChildElement("Vertices")->FirstChild(); vertex; vertex = vertex->NextSibling()) {
            vertices.emplace_back(vertex);
        }
        vertexCnt = vertices.size();
        if (vertexCnt) {
            vertexData = std::shared_ptr<T[]>(new T[vertexCnt]);
            std::copy(vertices.begin(), vertices.end(), vertexData.get());
        }
    }
private:
    std::shared_ptr<T[]> vertexData;
    uint32_t vertexCnt;
};

/* load vertex resource sync */
std::shared_ptr<ModelMetaBase> loadVertexFromFile(const char* fileName);

/* load model resource async */
void loadModelFromFileAsync(const char* fileName, std::function<void(std::shared_ptr<ModelMetaBase>)> callback);

#endif
