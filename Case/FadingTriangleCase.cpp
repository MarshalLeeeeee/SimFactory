#include "FadingTriangleCase.h"
#include "RenderTriangle.h"
#include "SimUtil.h"

FadingTriangleCase::FadingTriangleCase() :
    SimCase() {}

FadingTriangleCase::~FadingTriangleCase() {}

void FadingTriangleCase::do_update(ComPtr<ID3D11Device> dev, double simTime, double frameTime) {
	if (renderObjs.empty()) {
		std::string uuid;
		while (true) {
			uuid = GenerateRenderObjUUID();
			if (renderObjs.find(uuid) == renderObjs.end()) break;
		}
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
		std::shared_ptr<RenderTriangle> pTriangle = std::make_shared<RenderTriangle>(uuid, ps, cs);
		addRenderObj(pTriangle, dev);
	}
	else {
		double diff = simTime - static_cast<int>(simTime / PERIOD) * PERIOD;
		double absDiff = std::abs(diff - PERIOD * 0.5);
		double result = absDiff / PERIOD * 2.0;
		float c = static_cast<float>(result);
		DirectX::XMFLOAT4 cs[3] = {
			{ DirectX::XMFLOAT4(c, 0.0f, 0.0f, 1.0f) },
			{ DirectX::XMFLOAT4(0.0f, c, 0.0f, 1.0f) },
			{ DirectX::XMFLOAT4(0.0f, 0.0f, c, 1.0f) },
		};
		for (auto itr = renderObjs.begin(); itr != renderObjs.end(); ++itr) {
			itr->second->updateColor(cs);
		}
	}
}