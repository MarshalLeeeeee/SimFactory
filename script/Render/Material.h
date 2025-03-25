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
protected:
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

class VertexMaterial : public Material {
public:
	VertexMaterial();
	/* initialization of vertex shader, vertex layout and vertex constant buffer */
	bool init(ComPtr<ID3D11Device> dev, std::shared_ptr<MeshMeta> pMeshMeta, 
		const D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSz, std::vector<size_t>& bufferDataSizes);
	/* set vertex shader, vertex layout */
	void enableMaterial(ComPtr<ID3D11DeviceContext> devCon) const;
	/* set vertex buffer */
	void enableBuffer(ComPtr<ID3D11DeviceContext> devCon) const;
private:
	/* vertex shader file name */
	std::string vertexShaderFullname;
	/* vertex layout COM */
	ComPtr<ID3D11InputLayout> vertexLayout;
	/* vertex shader COM */
	ComPtr<ID3D11VertexShader> vertexShader;
};
class VertexMaterialBuffer {
public:
	VertexMaterialBuffer(std::shared_ptr<TransformBuffer> data);
	virtual void mapBuffer(ComPtr<ID3D11DeviceContext> devCon, std::shared_ptr<VertexMaterial> pVertexMaterial) const;
protected:
	std::shared_ptr<TransformBuffer> pTransformData;
};

class PixelMaterial : public Material {
public:
	PixelMaterial();
	/* initialization of pixel shader and vertex constant buffer */
	bool init(ComPtr<ID3D11Device> dev, std::shared_ptr<MeshMeta> pMeshMeta, 
		std::vector<size_t>& bufferDataSizes);
	/* set pixel shader */
	void enableMaterial(ComPtr<ID3D11DeviceContext> devCon) const;
	/* set pixel buffer */
	void enableBuffer(ComPtr<ID3D11DeviceContext> devCon) const;
private:
	/* pixel shader file name */
	std::string pixelShaderFullname;
	/* pixel shader COM */
	ComPtr<ID3D11PixelShader> pixelShader;
};
class PixelMaterialBuffer {
public:
	PixelMaterialBuffer();
	virtual void mapBuffer(ComPtr<ID3D11DeviceContext> devCon, std::shared_ptr<PixelMaterial> pPixelMaterial) const;
};

/////////////////////////////////////////////

class VMBPosColorRaw : public VertexMaterialBuffer {
public:
	VMBPosColorRaw(std::shared_ptr<TransformBuffer> data);
};

class PMBPosColorRaw : public PixelMaterialBuffer {
public:
	PMBPosColorRaw();
	virtual void mapBuffer(ComPtr<ID3D11DeviceContext> devCon, std::shared_ptr<PixelMaterial> pPixelMaterial) const;
protected:
	std::unique_ptr<PixelBuffer> pPixelData;
};

#endif
