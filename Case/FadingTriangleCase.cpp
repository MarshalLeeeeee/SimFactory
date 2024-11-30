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
		std::shared_ptr<RenderTriangle> pTriangle = std::make_shared<RenderTriangle>(uuid, 10.0);
		addRenderObj(pTriangle, dev);
	}
	else {
		for (auto itr = renderObjs.begin(); itr != renderObjs.end(); ++itr) {
			itr->second->update(simTime, frameTime);
		}
	}
}