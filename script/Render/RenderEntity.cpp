#include "RenderEntity.h"
#include "MathUtil.h"

RenderEntityBase::RenderEntityBase(std::string uuid, float x2y_) : 
    uuid(uuid), x2y(x2y_),
    posX(0.f), posY(0.f), posZ(0.f),
    scX(1.f), scY(1.f),
    angle(0.f),
    opacity(1.f)
    {
    pTfBufferData = std::make_shared<TransformBuffer>();
    updateTranformBuffer();
    pPxBufferData = std::make_shared<PixelBuffer>();
    updatePixelBuffer();
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

float RenderEntityBase::getAngle() const { 
    return angle;
}

void RenderEntityBase::setAngle(float a) {
    angle = a;
    if (angle > PI_2) angle -= PI_2;
    else if (angle < 0.) angle += PI_2;
}

void RenderEntityBase::updateTranformBuffer() {
    pTfBufferData->mat = getTransformMatrix(posX, posY, angle, scX, scY, x2y);
}

float RenderEntityBase::getOpacity() const { 
    return opacity;
}

void RenderEntityBase::setOpacity(float o) {
    opacity = o;
    if (opacity > 1.) opacity = 1.;
    else if (opacity < 0.) opacity = 0.;
}

float RenderEntityBase::getIntensity() const { 
    return intensity;
}

void RenderEntityBase::setIntensity(float i) {
    intensity = i;
    if (intensity > 1.) intensity = 1.;
    else if (intensity < 0.) intensity = 0.;
}

void RenderEntityBase::updatePixelBuffer() {
    pPxBufferData->data = DirectX::XMFLOAT4(opacity, intensity, 0.0f, 0.0f);
}
