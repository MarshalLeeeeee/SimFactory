#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const std::string& meshName) :
    indexCnt(0), indexAssetName(meshName) {}

IndexBuffer::~IndexBuffer() {}

bool IndexBuffer::init(ComPtr<ID3D11Device> dev, uint32_t indexCnt_, std::shared_ptr<DWORD[]> indices_) {
    indexCnt = indexCnt_;
    indices = std::make_unique<DWORD[]>(indexCnt);
    std::copy(indices_.get(), indices_.get() + indexCnt, indices.get());
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_IMMUTABLE;
    bd.ByteWidth = sizeof(DWORD) * indexCnt;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA initDataI;
    ZeroMemory(&initDataI, sizeof(initDataI));
    initDataI.pSysMem = indices.get();
    if (FAILED(dev->CreateBuffer(&bd, &initDataI, iBuffer.GetAddressOf()))) {
        return false;
    }
    return true;
}

void IndexBuffer::enableIndexBuffer(ComPtr<ID3D11DeviceContext> devCon) const {
    devCon->IASetIndexBuffer(iBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void IndexBuffer::drawIndex(ComPtr<ID3D11DeviceContext> devCon) const {
    devCon->DrawIndexed(indexCnt, 0, 0);
}

std::string IndexBuffer::getIndexAssetName() const {
    return indexAssetName;
}

//////////////////////

std::shared_ptr<IndexBuffer> IndexBufferPool::getIndexBuffer(std::string indexAssetName) const {
    auto itr = indexBuffers.find(indexAssetName);
    if (itr != indexBuffers.end()) return itr->second;
    else return nullptr;
}

void IndexBufferPool::addIndexBuffer(std::string indexAssetName, std::shared_ptr<IndexBuffer> pIndexBuffer) {
    auto itr = indexBuffers.find(indexAssetName);
    if (itr != indexBuffers.end()) return;
    indexBuffers[indexAssetName] = pIndexBuffer;
}

void IndexBufferPool::deRefIndexBuffer(std::string indexAssetName) {
    auto itr = indexBuffers.find(indexAssetName);
    if (itr == indexBuffers.end()) return;
    if (itr->second.use_count() <= 2) {
        indexBuffers.erase(itr);
    }
}
