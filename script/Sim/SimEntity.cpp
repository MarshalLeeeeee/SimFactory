#include "SimEntity.h"
#include "SimUtil.h"

SimEntity::SimEntity() : pRenderEntity(nullptr) {}

SimEntity::~SimEntity() {}

bool SimEntity::init(SimCase* pSimCase, std::string uuid, ComPtr<ID3D11Device> dev) {
	if (!initRenderEntity(pSimCase, dev)) return false;
	if (!initEntity()) return false;
	this->uuid = uuid;
	return true;
}

bool SimEntity::initRenderEntity(SimCase* pSimCase, ComPtr<ID3D11Device> dev) {
	return true;
}

bool SimEntity::initEntity() {
	return true;
}

void SimEntity::update(SimCase* pSimCase, double simTime, double frameTime) {
	updateProperty(pSimCase, simTime, frameTime);
    updateRenderEntity();
}

void SimEntity::updateRenderEntity() {}

std::string SimEntity::getUUID() const {
	return uuid;
}