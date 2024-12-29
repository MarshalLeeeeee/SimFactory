#include "FadingTriangleCase.h"
#include "SimUtil.h"
#include "TypeUtil.h"
#include "UIWidget.h"

FadingTriangle::FadingTriangle(FadingTriangleCase* pSimCase) :
	SimEntity(pSimCase), c(0.0f), d(1.0f) {}

FadingTriangle::~FadingTriangle() {}

bool FadingTriangle::initRenderEntity(ComPtr<ID3D11Device> dev) {
	D3D_PRIMITIVE_TOPOLOGY primitiveTopologyData[1] = {
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	};
	DWORD iData[3] = {
		0, 1, 2
	};
	DWORD* indicesData[1] = {
		iData
	};
	uint32_t indicesLenData[1] = {
		3
	};
	VertexPosColor vData[3] = {
		{ DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f) , DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.45f, -0.5, 0.0f) , DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.45f, -0.5f, 0.0f) , DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }
	};
	pRenderEntity = std::make_unique<RenderEntity<VertexPosColor>>(
		pSimCase,
		uuid,
		primitiveTopologyData,
		indicesData,
		indicesLenData,
		1,
		vData,
		3
	);
	bool res = pRenderEntity->init(dev);
	if (!res) pRenderEntity = nullptr;
	return res;
}

void FadingTriangle::updateProperty(double simTime, double frameTime) {
	float period = static_cast<FadingTriangleCase*>(pSimCase)->getPeriod();
	c += d * float(frameTime) / period;
	if (c > 1.0f) {
		d = -1.0f;
		c = 2.0f - c;
	}
	else if (c < 0.0f) {
		d = 1.0f;
		c = -c;
	}
}

void FadingTriangle::updateRenderEntity() {
	if (!pRenderEntity) return;
	DirectX::XMFLOAT4 cs[3] = {
		{ DirectX::XMFLOAT4(c, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.0f, c, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.0f, 0.0f, c, 1.0f) },
	};
	for (uint32_t i = 0; i < 3; ++i) {
		pRenderEntity->updateField(i, "color", cs[i]);
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

void FadingTriangleCase::doUpdate(ComPtr<ID3D11Device> dev, double simTime, double frameTime) {
	if (sliderUUID.empty()) {
		sliderUUID = GenerateUUID();
		pUI->addUIWidget(std::make_shared<UISliderFloat>(sliderUUID, "Slider", 1.0, 10.0));
		period = 1.0f;
	}
	else {
		Any a = pUI->getUIWidget(sliderUUID)->getValue();
		period = a.get<float>();
	}

	if (entities.empty()) {
		addEntity(std::make_shared<FadingTriangle>(this), dev);
	}
	else {
		updateEntities(simTime, frameTime);
	}
}

float FadingTriangleCase::getPeriod() const {
	return period;
}
