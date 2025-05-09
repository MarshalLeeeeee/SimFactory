#pragma once

#ifndef __MESH_H__
#define __MESH_H__

#include <Windows.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#include <memory>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "FileUtil.h"

class Mesh {
public:
	Mesh();
	~Mesh();
	/* initialization of mesh */
	bool init(ComPtr<ID3D11Device> dev, std::shared_ptr<MeshMeta> pMeshMeta, std::shared_ptr<VertexBuffer> pVertexBuffer,
		const D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSize, std::shared_ptr<TransformBuffer> pTransformData);
	/* render mesh */
	void render(ComPtr<ID3D11DeviceContext> devCon) const;
	/* update vertex shader buffer data */
	void updateVertexMaterialBuffer(std::string& bufferName, const Any& any);
	/* update pixel shader buffer data */
	void updatePixelMaterialBuffer(std::string& bufferName, const Any& any);
private:
	/* primitive topology */
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology;
	/* reference of vertex buffer instance */
	std::shared_ptr<VertexBuffer> pVertexBuffer;
	/* reference of index buffer instance */
	std::shared_ptr<IndexBuffer> pIndexBuffer;
	/* reference of vertex material instance */
	std::shared_ptr<VertexMaterial> pVertexMaterial;
	/* reference of vertex material buffer instance */
	std::unique_ptr<VertexMaterialBuffer> pVertexMaterialBuffer;
	/* reference of pixel material instance */
	std::shared_ptr<PixelMaterial> pPixelMaterial;
	/* reference of pixel material buffer instance */
	std::unique_ptr<PixelMaterialBuffer> pPixelMaterialBuffer;
	/* if is ready to be rendered */
	bool loaded;
};

#endif
