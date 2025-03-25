#include "SimEntity.h"
#include "SimUtil.h"

SimEntity::SimEntity() : pModel(nullptr) {}

SimEntity::~SimEntity() {}

bool SimEntity::init(ComPtr<ID3D11Device> dev, SimCase* pSimCase, std::string uuid) {
	if (!initEntity()) return false;
	this->uuid = uuid;
	initModel(dev, pSimCase);
	return true;
}

void SimEntity::initModel(ComPtr<ID3D11Device> dev, SimCase* pSimCase) {}

bool SimEntity::initEntity() {
	return true;
}

void SimEntity::update(SimCase* pSimCase, double simTime, double frameTime) {
	updateProperty(pSimCase, simTime, frameTime);
    updateModel();
}

void SimEntity::updateModel() {}

std::string SimEntity::getUUID() const {
	return uuid;
}

void SimEntity::render(ComPtr<ID3D11DeviceContext> devCon) const {
	if (!pModel) return;
	pModel->render(devCon);
}