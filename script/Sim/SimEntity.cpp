#include "SimEntity.h"
#include "SimUtil.h"

SimEntity::SimEntity() : pRenderEntity(nullptr) {}

SimEntity::~SimEntity() {}

bool SimEntity::init(SimCase* pSimCase, ComPtr<ID3D11Device> dev) {
	if (!initUUID(pSimCase)) return false;
	if (!initRenderEntity(pSimCase, dev)) return false;
	if (!initEntity()) return false;
	return true;
}

bool SimEntity::initUUID(SimCase* pSimCase) {
	std::string _uuid;
	for (int i = 0; i < 50; ++i) {
		_uuid = generateUUID();
		if (!pSimCase->hasRenderObj(_uuid)) {
			uuid = _uuid;
			return true;
		}
	}
	return false;
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