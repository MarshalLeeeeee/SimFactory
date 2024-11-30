#include "SimWorld.h"
#include "SimUtil.h"
#include "RenderTriangle.h"

SimWorld::SimWorld() {
	startTime = std::chrono::high_resolution_clock::now();
	updateTime = startTime;
}

SimWorld::~SimWorld() {}

void SimWorld::update(ComPtr<ID3D11Device> dev) {
	auto currTime = std::chrono::high_resolution_clock::now();
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
		double simTime = static_cast<std::chrono::duration<double>>(currTime - startTime).count();
		double frameTime = static_cast<std::chrono::duration<double>>(currTime - updateTime).count();
		for (auto itr = renderObjs.begin(); itr != renderObjs.end(); ++itr) {
			itr->second->update(simTime, frameTime);
		}
	}
	updateTime = currTime;
}

void SimWorld::render(ComPtr<ID3D11DeviceContext> devCon) {
	for (auto itr = renderObjs.begin(); itr != renderObjs.end(); ++itr) {
		itr->second->render(devCon);
	}
}

void SimWorld::addRenderObj(std::shared_ptr<RenderObj> pRenderObj, ComPtr<ID3D11Device> dev) {
	std::string uuid = pRenderObj->getUUID();
	renderObjs[uuid] = pRenderObj;
	if (!renderObjs[uuid]->init(dev)) {
		removeRenderObj(uuid);
	}
}

void SimWorld::removeRenderObj(std::string uuid) {
	renderObjs.erase(uuid);
}