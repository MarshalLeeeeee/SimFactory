#include "RenderEntity.h"

RenderEntityBase::RenderEntityBase(std::string uuid) : 
    uuid(uuid),
    posX(0.5f), posY(0.5f), posZ(0.f),
    scX(1.f), scY(1.f),
    angle(1.f),
    opacity(1.f),
    vis(true)
    {
    pTfBufferData = std::make_shared<TransformBuffer>();
    updateTranformBuffer();
}

RenderEntityBase::~RenderEntityBase() {}

bool RenderEntityBase::init(
    SimCase* pSimCase,
    ComPtr<ID3D11Device> dev,
    D3D_PRIMITIVE_TOPOLOGY* primitiveTopologyData,
    DWORD** indicesData,
    uint32_t* indicesLenData,
    uint32_t renderObjCnt
    ) {
    if (!initRenderObj(
        pSimCase,
        dev,
        primitiveTopologyData,
        indicesData,
        indicesLenData,
        renderObjCnt
    )) return false;
    return true;
}


