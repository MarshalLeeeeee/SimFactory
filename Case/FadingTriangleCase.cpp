#include "FadingTriangleCase.h"
#include "SimUtil.h"
#include "TypeUtil.h"
#include "UIWidget.h"

FadingTriangle::FadingTriangle(SimCase* pSimCase) :
	SimEntity(pSimCase), c(0.0f), d(1.0f),
	vertexCnt(3), indexCnt(3) {
		VertexPosColor vData[3] = {
			{ DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f) , DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ DirectX::XMFLOAT3(0.45f, -0.5, 0.0f) , DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ DirectX::XMFLOAT3(-0.45f, -0.5f, 0.0f) , DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }
		};
		DWORD iData[3] = {
			0, 1, 2
		};
		vertices = std::make_unique<VertexPosColor[]>(vertexCnt);
        for (int i = 0; i < vertexCnt; ++i) {
            vertices[i] = vData[i];
        }
        indices = std::make_unique<DWORD[]>(indexCnt);
        for (int i = 0; i < indexCnt; ++i) {
            indices[i] = iData[i];
        }
	}

bool FadingTriangle::initRenderObj(ComPtr<ID3D11Device> dev) {
	return pSimCase->addRenderObj(
		std::make_shared<RenderGeometry<VertexPosColor>>(
			uuid,
			L"vs.hlsl",
			L"ps.hlsl",
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			vertices,
			vertexCnt,
			indices,
			indexCnt
		),
		dev
	);
}

bool FadingTriangle::initEntity() {
	return true;
}

FadingTriangle::~FadingTriangle() {}

void FadingTriangle::updateProperty(double simTime, double frameTime) {
	float period = static_cast<FadingTriangleCase*>(pSimCase)->getPeriod();
	c += d * frameTime / period;
	if (c > 1.0f) {
		d = -1.0f;
		c = 2.0 - c;
	}
	else if (c < 0.0f) {
		d = 1.0f;
		c = -c;
	}
}

void FadingTriangle::updateRenderObj(std::shared_ptr<RenderObj> ro) {
	DirectX::XMFLOAT4 cs[3] = {
		{ DirectX::XMFLOAT4(c, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.0f, c, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.0f, 0.0f, c, 1.0f) },
	};
	for (uint32_t i = 0; i < 3; ++i) {
		vertices[i].color = cs[i];
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
	if (sliderUUID.empty()) {
		sliderUUID = GenerateRenderObjUUID();
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