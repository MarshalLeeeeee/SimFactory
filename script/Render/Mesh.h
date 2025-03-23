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
#include "Material.h"
#include "FileUtil.h"
#include "GraphicsUtil.h"

class Mesh {
public:
	Mesh();
	bool init(std::shared_ptr<MeshMeta>, std::shared_ptr<VertexBuffer>, ComPtr<ID3D11Device>, const D3D11_INPUT_ELEMENT_DESC*, UINT, std::shared_ptr<TransformBuffer>);
	void render(ComPtr<ID3D11DeviceContext> devCon) const;
private:
	std::shared_ptr<VertexBuffer> pVertexBuffer;
	ComPtr<ID3D11Buffer> iBuffer;
	uint32_t indiceCnt;
	std::shared_ptr<VertexMaterial> pVertexMaterial;
	std::shared_ptr<VertexMaterialBuffer> pVertexMaterialBuffer;
	std::shared_ptr<PixelMaterial> pPixelMaterial;
	std::shared_ptr<PixelMaterialBuffer> pPixelMaterialBuffer;
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology;
	bool loaded;
};

#endif
