#pragma once

#ifndef __INDEXBUFFER_H__
#define __INDEXBUFFER_H__


#include <Windows.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#include <string>
#include <memory>
#include <unordered_map>
#include "FileUtil.h"

class IndexBuffer {
public:
	IndexBuffer(const std::string& meshName);
	~IndexBuffer();
	/* initialization of index buffer and data */
	bool init(ComPtr<ID3D11Device> dev, uint32_t indexCnt, std::shared_ptr<DWORD[]> indices);
	/* set iBuffer in pipeline */
	void enableIndexBuffer(ComPtr<ID3D11DeviceContext> devCon) const;
	/* draw index */
	void drawIndex(ComPtr<ID3D11DeviceContext> devCon) const;
	/* get index asset name */
	std::string getIndexAssetName() const;
private:
	/* index buffer */
	ComPtr<ID3D11Buffer> iBuffer;
	/* index data */
	std::unique_ptr<DWORD[]> indices;
	/* index count */
	uint32_t indexCnt;
	/* index asset name */
	std::string indexAssetName;
};

class IndexBufferPool {
public:
	static IndexBufferPool& getInstance() {
		static IndexBufferPool instance;
		return instance;
	}
private:
	IndexBufferPool() {}
	~IndexBufferPool() {}
	IndexBufferPool(const IndexBufferPool&) = delete;
	IndexBufferPool(IndexBufferPool&&) = delete;
	IndexBufferPool& operator=(const IndexBufferPool&) = delete;
	IndexBufferPool& operator=(IndexBufferPool&&) = delete;

public:
	/* get cached IndexBuffer if exists */
	std::shared_ptr<IndexBuffer> getIndexBuffer(std::string indexAssetName) const;
	/* add new IndexBuffer in pool (only when indexAssetName does not exist) */
	void addIndexBuffer(std::string indexAssetName, std::shared_ptr<IndexBuffer> pIndexBuffer);
	/* de reference IndexBuffer */
	void deRefIndexBuffer(std::string indexAssetName);
private:
	/* cached IndexBuffer where key is index asset name */
	std::unordered_map<std::string, std::shared_ptr<IndexBuffer>> indexBuffers;
};

#endif
