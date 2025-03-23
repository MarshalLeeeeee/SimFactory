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
	const std::vector<ComPtr<ID3D11Buffer>>& getBuffers() const;
	const std::vector<size_t>& getBufferSizes() const;
	uint32_t getBufferCnt() const;
protected:
	std::vector<ComPtr<ID3D11Buffer>> buffers;
	std::vector<size_t> bufferSizes;
	uint32_t bufferCnt;
};

class VertexMaterial : public Material {
public:
	VertexMaterial();
	bool init(ComPtr<ID3D11Device> dev, std::shared_ptr<MeshMeta> pMeshMeta, const D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSz, std::vector<size_t>& bufferDataSzs);
	void render(ComPtr<ID3D11DeviceContext> devCon) const;
private:
	std::string layoutName;
	ComPtr<ID3D11InputLayout> vLayout;
	std::string vertexShaderName;
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
	bool init(ComPtr<ID3D11Device> dev, std::shared_ptr<MeshMeta> pMeshMeta, std::vector<size_t>& bufferDataSzs);
	void render(ComPtr<ID3D11DeviceContext> devCon) const;
private:
	std::string layoutName;
	std::string pixelShaderName;
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
