#pragma once

#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <Windows.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#include <memory>
#include <vector>
#include "FileUtil.h"
#include "TypeUtil.h"

class Material {
public:
	Material();
	/* get dx COM buffers */
	const std::vector<ComPtr<ID3D11Buffer>>& getBuffers() const;
	/* get buffer data (cpp cls) size */
	const std::vector<size_t>& getBufferDataSizes() const;
	/* get the count of dx COM buffers */
	uint32_t getBufferCnt() const;
	/* initialization of constant buffers */
	bool initBuffers(ComPtr<ID3D11Device> dev, std::vector<size_t>& bufferDataSizes);
protected:
	/* vector of dx COM buffers */
	std::vector<ComPtr<ID3D11Buffer>> buffers;
	/* vector of buffer data (cpp cls) size */
	std::vector<size_t> bufferDataSizes;
	/* count of dx COM buffers */
	uint32_t bufferCnt;
};

/////////////////////////////////////////////////////////////

class VertexMaterial : public Material {
public:
	VertexMaterial(const std::string& vertexShaderFilename);
	/* initialization of vertex shader, vertex layout */
	bool initShader(ComPtr<ID3D11Device> dev, const D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSz);
	/* set vertex shader, vertex layout */
	void enableMaterial(ComPtr<ID3D11DeviceContext> devCon) const;
	/* set vertex buffer */
	void enableBuffer(ComPtr<ID3D11DeviceContext> devCon) const;
	/* get vertex shader filename */
	std::string getVertexShaderFilename() const;
private:
	/* vertex shader file name */
	std::string vertexShaderFilename;
	/* vertex layout COM */
	ComPtr<ID3D11InputLayout> vertexLayout;
	/* vertex shader COM */
	ComPtr<ID3D11VertexShader> vertexShader;
};
class VertexMaterialBuffer {
public:
	VertexMaterialBuffer(std::shared_ptr<TransformBuffer> data);
	/* init vertex material shader buffers */
	bool initMaterialBuffer(ComPtr<ID3D11Device> dev, std::shared_ptr<VertexMaterial> pVertexMaterial);
	/* map data to constant vertex shader buffer */
	virtual void mapBuffer(ComPtr<ID3D11DeviceContext> devCon, std::shared_ptr<VertexMaterial> pVertexMaterial) const;
	/* update buffer data */
	virtual void updateBufferData(std::string& bufferName, const Any& any);
protected:
	/* get constant buffer data type size */
	virtual void getBufferDataSizes(std::vector<size_t>& vertexShaderBufferDataSizes);
	/* 4*4 metrix model transformation */
	std::shared_ptr<TransformBuffer> pTransformData;
};

class VertexMaterialPool {
public:
	static VertexMaterialPool& getInstance() {
		static VertexMaterialPool instance;
		return instance;
	}
private:
	VertexMaterialPool() {}
	~VertexMaterialPool() {}
	VertexMaterialPool(const VertexMaterialPool&) = delete;
	VertexMaterialPool(VertexMaterialPool&&) = delete;
	VertexMaterialPool& operator=(const VertexMaterialPool&) = delete;
	VertexMaterialPool& operator=(VertexMaterialPool&&) = delete;

public:
	/* get cached VertexMaterial if exists */
	std::shared_ptr<VertexMaterial> getVertexMaterial(std::string vertexShaderFilename) const;
	/* add new VertexMaterial in pool (only when vertexShaderFilename does not exist) */
	void addVertexMaterial(std::string vertexShaderFilename, std::shared_ptr<VertexMaterial> pVertexMaterial);
	/* de reference VertexMaterial */
	void deRefVertexMaterial(std::string vertexShaderFilename);
private:
	/* cached VertexMaterial where key is vertex shader filename */
	std::unordered_map<std::string, std::shared_ptr<VertexMaterial>> vertexMaterials;
};

/////////////////////////////////////////////////////////////

class PixelMaterial : public Material {
public:
	PixelMaterial(const std::string& pixelShaderFilename);
	/* initialization of pixel shader */
	bool initShader(ComPtr<ID3D11Device> dev);
	/* set pixel shader */
	void enableMaterial(ComPtr<ID3D11DeviceContext> devCon) const;
	/* set pixel buffer */
	void enableBuffer(ComPtr<ID3D11DeviceContext> devCon) const;
	/* get pixel shader filename */
	std::string getPixelShaderFilename() const;
private:
	/* pixel shader file name */
	std::string pixelShaderFilename;
	/* pixel shader COM */
	ComPtr<ID3D11PixelShader> pixelShader;
};
class PixelMaterialBuffer {
public:
	PixelMaterialBuffer();
	/* init pixel material shader buffers */
	bool initMaterialBuffer(ComPtr<ID3D11Device> dev, std::shared_ptr<PixelMaterial> pPixelMaterial);
	/* map data to constant pixel shader buffer */
	virtual void mapBuffer(ComPtr<ID3D11DeviceContext> devCon, std::shared_ptr<PixelMaterial> pPixelMaterial) const;
	/* update buffer data */
	virtual void updateBufferData(std::string& bufferName, const Any& any);
protected:
	/* get constant buffer data type size */
	virtual void getBufferDataSizes(std::vector<size_t>& pixelShaderBufferDataSizes);
};

class PixelMaterialPool {
public:
	static PixelMaterialPool& getInstance() {
		static PixelMaterialPool instance;
		return instance;
	}
private:
	PixelMaterialPool() {}
	~PixelMaterialPool() {}
	PixelMaterialPool(const PixelMaterialPool&) = delete;
	PixelMaterialPool(PixelMaterialPool&&) = delete;
	PixelMaterialPool& operator=(const PixelMaterialPool&) = delete;
	PixelMaterialPool& operator=(PixelMaterialPool&&) = delete;

public:
	/* get cached PixelMaterial if exists */
	std::shared_ptr<PixelMaterial> getPixelMaterial(std::string pixelShaderFilename) const;
	/* add new PixelMaterial in pool (only when pixelShaderFilename does not exist) */
	void addPixelMaterial(std::string pixelShaderFilename, std::shared_ptr<PixelMaterial> pPixelMaterial);
	/* de reference PixelMaterial */
	void deRefPixelMaterial(std::string pixelShaderFilename);
private:
	/* cached PixelMaterial where key is pixel shader filename */
	std::unordered_map<std::string, std::shared_ptr<PixelMaterial>> pixelMaterials;
};

/////////////////////////////////////////////

class VMBPosColorRaw : public VertexMaterialBuffer {
public:
	VMBPosColorRaw(std::shared_ptr<TransformBuffer> data);
};

class PMBPosColorRaw : public PixelMaterialBuffer {
public:
	PMBPosColorRaw();
	/* map data to constant pixel shader buffer */
	virtual void mapBuffer(ComPtr<ID3D11DeviceContext> devCon, std::shared_ptr<PixelMaterial> pPixelMaterial) const;
	/* update buffer data */
	virtual void updateBufferData(std::string& bufferName, const Any& any);
protected:
	/* get constant buffer data type size */
	virtual void getBufferDataSizes(std::vector<size_t>& pixelShaderBufferDataSizes);
	/* intensity and opacity */
	std::unique_ptr<PixelBuffer> pPixelData;
};

/////////////////////////////////////////////

/* get vertex material buffer given vertex shader filename */
std::unique_ptr<VertexMaterialBuffer> createVertexMaterialBuffer(const std::string& vertexShaderFilename, std::shared_ptr<TransformBuffer> pTransformData);

/* get pixel material buffer given pixel shader filename */
std::unique_ptr<PixelMaterialBuffer> createPixelMaterialBuffer(const std::string& pixelShaderFilename);

#endif
