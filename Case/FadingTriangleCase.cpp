#include "FadingTriangleCase.h"
#include "SimUtil.h"
#include "TypeUtil.h"
#include "UIWidget.h"

FadingTriangleCase::FadingTriangle::FadingTriangle(SimCase* pSimCase) :
	SimEntity(pSimCase), c(0.0f) {}

bool FadingTriangleCase::FadingTriangle::initRenderObj(ComPtr<ID3D11Device> dev) {
	VertexPosColor vertices[3] = {
		{ DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f) , DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.45f, -0.5, 0.0f) , DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.45f, -0.5f, 0.0f) , DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) }
	};
	DWORD indices[3] = {
		0, 1, 2
	};
	return pSimCase->addRenderObj(
		std::make_shared<RenderGeometry<VertexPosColor>>(
			uuid,
			L"vs.hlsl",
			L"ps.hlsl",
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			vertices,
			3,
			indices,
			3
		),
		dev
	);
}

bool FadingTriangleCase::FadingTriangle::initEntity() {
	return true;
}

FadingTriangleCase::FadingTriangle::~FadingTriangle() {}

void FadingTriangleCase::FadingTriangle::updateProperty(double simTime, double frameTime) {
	float period = static_cast<FadingTriangleCase*>(pSimCase)->getPeriod();
	double diff = simTime - static_cast<int>(simTime / period) * period;
	double absDiff = std::abs(diff - period * 0.5);
	double result = absDiff / period * 2.0;
	c = static_cast<float>(result);
}

void FadingTriangleCase::FadingTriangle::updateRenderObj(std::shared_ptr<RenderObj> ro) {
	DirectX::XMFLOAT4 cs[3] = {
		{ DirectX::XMFLOAT4(c, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.0f, c, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.0f, 0.0f, c, 1.0f) },
	};
	for (uint32_t i = 0; i < 3; ++i) {
		ro->updateField(i, "color", Any(cs[i]));
	}
}
	

FadingTriangleCase::FadingTriangleCase() :
    SimCase(), period(1.0f) {}

FadingTriangleCase::~FadingTriangleCase() {}

int FadingTriangleCase::getScreenWidth() const {
	return 1280;
}

int FadingTriangleCase::getScreenHeight() const {
	return 960;
}

bool FadingTriangleCase::needUI() const {
	return true;
}

float FadingTriangleCase::getPeriod() const {
	return period;
}

void FadingTriangleCase::doUpdate(ComPtr<ID3D11Device> dev, double simTime, double frameTime) {
	/*if (sliderUUID.empty()) {
		sliderUUID = GenerateRenderObjUUID();
		pUI->addUIWidget(std::make_shared<UISliderFloat>(sliderUUID, "Slider", 0.0, 10.0));
	}
	else {
		Any a;
		pUI->getUIWidget(sliderUUID)->getValue(a);
		period = a.get<float>();
	}*/

	if (entities.empty()) {
		addEntity(std::make_shared<FadingTriangle>(this), dev);
	}
	else {
		updateEntities(simTime, frameTime);
	}
}