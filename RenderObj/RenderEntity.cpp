#include "RenderEntity.h"

RenderEntityBase::RenderEntityBase(SimCase* pSimCase, std::string uuid) :
    pSimCase(pSimCase), uuid(uuid) {}

RenderEntityBase::~RenderEntityBase() {}

bool RenderEntityBase::init(ComPtr<ID3D11Device> dev) {
    if (!initRenderObj(dev)) return false;
    return true;
}


