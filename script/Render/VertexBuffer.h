#pragma once

#ifndef __VERTEXBUFFER_H__
#define __VERTEXBUFFER_H__

#include <Windows.h>
#include <wrl/client.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#include <string>
#include <memory>
#include <unordered_map>
#include "FileUtil.h"

class VertexBuffer {
public:
    VertexBuffer(const std::string& vertexFilename);
    virtual ~VertexBuffer();
    /* initialize vertex data and immutable vertex buffer */
    bool init(ComPtr<ID3D11Device> dev, std::shared_ptr<ModelMetaBase> pModelMeta);
    /* set vBuffer in pipeline */
    void enableVertexBuffer(ComPtr<ID3D11DeviceContext> devCon) const;
    /* get vertex filename */
    std::string getVertexFilename() const;
private:
    /* vertex buffer */
    ComPtr<ID3D11Buffer> vBuffer;
    /* vertex data */
    void* vertices;
    /* size of vertex data type */
    UINT stride;
    /* vertex filename */
    std::string vertexFilename;
};

class VertexBufferPool {
public:
    static VertexBufferPool& getInstance() {
        static VertexBufferPool instance;
        return instance;
    }
private:
    VertexBufferPool() {}
    ~VertexBufferPool() {}
    VertexBufferPool(const VertexBufferPool&) = delete;
    VertexBufferPool(VertexBufferPool&&) = delete;
    VertexBufferPool& operator=(const VertexBufferPool&) = delete;
    VertexBufferPool& operator=(VertexBufferPool&&) = delete;

public:
    /* get cached VertexBuffer if exists */
    std::shared_ptr<VertexBuffer> getVertexBuffer(std::string vertexFilename) const;
    /* add new VertexBuffer in pool (only when vertexFilename does not exist) */
    void addVertexBuffer(std::string vertexFilename, std::shared_ptr<VertexBuffer> pVertexBuffer);
    /* de reference VertexBuffer */
    void deRefVertexBuffer(std::string vertexFilename);
private:
    /* cached VertexBuffer where key is vertex resource filename */
    std::unordered_map<std::string, std::shared_ptr<VertexBuffer>> vertexBuffers;
};

#endif
