#include "FadingTriangleCase.h"
#include "RenderTriangle.h"
#include "SimUtil.h"

FadingTriangleCase::FadingTriangle::FadingTriangle(ComPtr<ID3D11Device> dev, SimCase* pSimCase) :
	SimEntity(dev, pSimCase) {
	initRenderObj(dev);
}

void FadingTriangleCase::FadingTriangle::initRenderObj(ComPtr<ID3D11Device> dev) {
	DirectX::XMFLOAT3 ps[3] = {
		{ DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f) },
		{ DirectX::XMFLOAT3(0.45f, -0.5, 0.0f) },
		{ DirectX::XMFLOAT3(-0.45f, -0.5f, 0.0f) },
	};
	DirectX::XMFLOAT4 cs[3] = {
		{ DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
	};
	pSimCase->addRenderObj(
		std::make_shared<RenderTriangle>(uuid, ps, cs),
		dev
	);
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
	ro->updateColor(cs);
}

FadingTriangleCase::FadingTriangleCase() :
    SimCase(), fadingTriangle(nullptr){}

FadingTriangleCase::~FadingTriangleCase() {}

void FadingTriangleCase::doUpdate(ComPtr<ID3D11Device> dev, double simTime, double frameTime) {
	if (renderObjs.empty()) {
		addEntity(std::make_shared<FadingTriangle>(dev, this));
	}
	else {
		updateEntities(simTime, frameTime);
	}
}