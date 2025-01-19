#include "RenderEntity.h"

RenderEntityBase::RenderEntityBase(std::string uuid) : uuid(uuid) {}

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


