#include "FadingTriangleCase.h"
#include "SimUtil.h"
#include "TypeUtil.h"

FadingTriangleCase::FadingTriangle::FadingTriangle(SimCase* pSimCase) :
	SimEntity(pSimCase) {}

bool FadingTriangleCase::FadingTriangle::initRenderObj(ComPtr<ID3D11Device> dev) {
	VertexPosColor vertices[3] = {
		{ DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f) , DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.45f, -0.5, 0.0f) , DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.45f, -0.5f, 0.0f) , DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) }
	};
	return pSimCase->addRenderObj(
		std::make_shared<RenderGeometry<VertexPosColor>>(
			uuid,
			L"vs.hlsl",
			L"ps.hlsl",
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			vertices,
			3
		),
		dev
	);
}

bool FadingTriangleCase::FadingTriangle::initEntity() {
	return true;
}

FadingTriangleCase::FadingTriangle::~FadingTriangle() {}

void FadingTriangleCase::FadingTriangle::updateRenderObj(std::shared_ptr<RenderObj> ro, double simTime, double frameTime) {
	double diff = simTime - static_cast<int>(simTime / PERIOD) * PERIOD;
	double absDiff = std::abs(diff - PERIOD * 0.5);
	double result = absDiff / PERIOD * 2.0;
	float c = static_cast<float>(result);
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
    SimCase() {}

FadingTriangleCase::~FadingTriangleCase() {}

int FadingTriangleCase::getScreenWidth() const {
	return 1280;
};

int FadingTriangleCase::getScreenHeight() const {
	return 960;
};

void FadingTriangleCase::doUpdate(ComPtr<ID3D11Device> dev, double simTime, double frameTime) {
	if (entities.empty()) {
		addEntity(std::make_shared<FadingTriangle>(this), dev);
	}
	else {
		updateEntities(simTime, frameTime);
	}
}