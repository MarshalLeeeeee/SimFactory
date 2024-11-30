#include "SimCase.h"

SimCase::SimCase() {
	startTime = std::chrono::high_resolution_clock::now();
	updateTime = startTime;
}

SimCase::~SimCase() {}

void SimCase::update(ComPtr<ID3D11Device> dev) {
	auto currTime = std::chrono::high_resolution_clock::now();
    double simTime = static_cast<std::chrono::duration<double>>(currTime - startTime).count();
    double frameTime = static_cast<std::chrono::duration<double>>(currTime - updateTime).count();
    do_update(dev, simTime, frameTime);
	updateTime = currTime;
}

void SimCase::render(ComPtr<ID3D11DeviceContext> devCon) {
	for (auto itr = renderObjs.begin(); itr != renderObjs.end(); ++itr) {
		itr->second->render(devCon);
	}
}

void SimCase::addRenderObj(std::shared_ptr<RenderObj> pRenderObj, ComPtr<ID3D11Device> dev) {
	std::string uuid = pRenderObj->getUUID();
	renderObjs[uuid] = pRenderObj;
	if (!renderObjs[uuid]->init(dev)) {
		removeRenderObj(uuid);
	}
}

void SimCase::removeRenderObj(std::string uuid) {
	renderObjs.erase(uuid);
}