
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const std::string& vertexFilename_) :
    vertexFilename(vertexFilename_), vertices(nullptr), stride(0) {}

VertexBuffer::~VertexBuffer() {
	if (vertices) {
		::operator delete(vertices);
	}
}

bool VertexBuffer::init(ComPtr<ID3D11Device> dev, std::shared_ptr<ModelMetaBase> pModelMeta) {
	if (!pModelMeta) return false;

	// vertices data
	size_t vertexDataByteWidth = pModelMeta->getVertexDataByteWidth();
	if (vertexDataByteWidth == 0) return false;
	vertices = ::operator new(vertexDataByteWidth);
	if (!vertices) return false;
	memcpy(vertices, pModelMeta->getVertexData(), vertexDataByteWidth);

	// vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = vertexDataByteWidth;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertices;
	if (FAILED(dev->CreateBuffer(&bd, &initData, vBuffer.GetAddressOf()))) {
		return false;
	}
	stride = pModelMeta->getVertexDataTypeSize();
	return true;
}

void VertexBuffer::enableVertexBuffer(ComPtr<ID3D11DeviceContext> devCon) const {
	UINT offset = 0;
	devCon->IASetVertexBuffers(0, 1, vBuffer.GetAddressOf(), &stride, &offset);
}

std::string VertexBuffer::getVertexFilename() const {
    return vertexFilename;
}

//////////////////////

std::shared_ptr<VertexBuffer> VertexBufferPool::getVertexBuffer(std::string vertexFilename) const {
    auto itr = vertexBuffers.find(vertexFilename);
    if (itr != vertexBuffers.end()) return itr->second;
    else return nullptr;
}

void VertexBufferPool::addVertexBuffer(std::string vertexFilename, std::shared_ptr<VertexBuffer> pVertexBuffer) {
    auto itr = vertexBuffers.find(vertexFilename);
    if (itr != vertexBuffers.end()) return;
    vertexBuffers[vertexFilename] = pVertexBuffer;
}

void VertexBufferPool::deRefVertexBuffer(std::string vertexFilename) {
    auto itr = vertexBuffers.find(vertexFilename);
    if (itr == vertexBuffers.end()) return;
    if (itr->second.use_count() <= 2) {
        vertexBuffers.erase(itr);
    }
}
