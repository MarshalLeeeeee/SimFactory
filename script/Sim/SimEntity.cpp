#include "SimEntity.h"
#include "SimUtil.h"

SimEntity::SimEntity(SimCase* pSimCase) :
	pSimCase(pSimCase), pRenderEntity(nullptr) {}

SimEntity::~SimEntity() {}

bool SimEntity::init(ComPtr<ID3D11Device> dev) {
	if (!initUUID()) return false;
	if (!initRenderEntity(dev)) return false;
	if (!initEntity()) return false;
	return true;
}

bool SimEntity::initUUID() {
	std::string _uuid;
	for (int i = 0; i < 50; ++i) {
		_uuid = GenerateUUID();
		if (!pSimCase->hasRenderObj(_uuid)) {
			uuid = _uuid;
			return true;
		}
	}
	return false;
}

bool SimEntity::initRenderEntity(ComPtr<ID3D11Device> dev) {
	return true;
}

bool SimEntity::initEntity() {
	return true;
}

void SimEntity::update(double simTime, double frameTime) {
	updateProperty(simTime, frameTime);
    updateRenderEntity();
}

void SimEntity::updateRenderEntity() {}

std::string SimEntity::getUUID() const {
	return uuid;
}