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
	doUpdate(dev, simTime, frameTime);
	updateTime = currTime;
}

void SimCase::doUpdate(ComPtr<ID3D11Device> dev, double simTime, double frameTime) {}

void SimCase::updateEntities(double simTime, double frameTime) {
	for (auto itr = entities.begin(); itr != entities.end(); ++itr) {
		itr->second->update(simTime, frameTime);
	}
}

void SimCase::render(ComPtr<ID3D11DeviceContext> devCon) {
	for (auto itr = renderObjs.begin(); itr != renderObjs.end(); ++itr) {
		itr->second->render(devCon);
	}
}

void SimCase::addEntity(std::shared_ptr<SimEntity> pSimEntity) {
	std::string uuid = pSimEntity->getUUID();
	entities[uuid] = pSimEntity;
}

void SimCase::removeEntity(std::string uuid) {
	entities.erase(uuid);
}

bool SimCase::hasEntity(std::string uuid) const {
	return entities.find(uuid) != entities.end();
}

std::shared_ptr<SimEntity> SimCase::getEntity(std::string uuid) const {
	if (!hasEntity(uuid)) return nullptr;
	else return entities.at(uuid);
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

bool SimCase::hasRenderObj(std::string uuid) const {
	return renderObjs.find(uuid) != renderObjs.end();
}

std::shared_ptr<RenderObj> SimCase::getRenderObj(std::string uuid) const {
	if (!hasRenderObj(uuid)) return nullptr;
	else return renderObjs.at(uuid);
}
