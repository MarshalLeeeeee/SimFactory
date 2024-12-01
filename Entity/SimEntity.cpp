#include "SimEntity.h"
#include "SimUtil.h"

SimEntity::SimEntity(SimCase* pSimCase) :
	pSimCase(pSimCase) {}

SimEntity::~SimEntity() {
	pSimCase->removeRenderObj(uuid);
}

bool SimEntity::init(ComPtr<ID3D11Device> dev) {
	if (!initUUID()) return false;
	if (!initRenderObj(dev)) return false;
	if (!initEntity()) return false;
	return true;
}

void SimEntity::update(double simTime, double frameTime) {
	std::shared_ptr<RenderObj> ro = pSimCase->getRenderObj(uuid);
	if (!ro) return;

    updateRenderObj(ro, simTime, frameTime);
}

bool SimEntity::initUUID() {
	std::string _uuid;
	for (int i = 0; i < 50; ++i) {
		_uuid = GenerateRenderObjUUID();
		if (!pSimCase->hasRenderObj(_uuid)) {
			uuid = _uuid;
			return true;
		}
	}
	return false;
}

std::string SimEntity::getUUID() const {
	return uuid;
}