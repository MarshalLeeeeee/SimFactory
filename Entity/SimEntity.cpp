#include "SimEntity.h"
#include "SimUtil.h"

SimEntity::SimEntity(ComPtr<ID3D11Device> dev, SimCase* pSimCase) :
	pSimCase(pSimCase) {
	initUUID();
}

SimEntity::~SimEntity() {}

void SimEntity::update(double simTime, double frameTime) {
	std::shared_ptr<RenderObj> ro = pSimCase->getRenderObj(uuid);
	if (!ro) return;

    updateRenderObj(ro, simTime, frameTime);
}

void SimEntity::initUUID() {
	std::string _uuid;
	while (true) {
		_uuid = GenerateRenderObjUUID();
		if (!pSimCase->hasRenderObj(_uuid)) break;
	}
	uuid = _uuid;
}

std::string SimEntity::getUUID() const {
	return uuid;
}