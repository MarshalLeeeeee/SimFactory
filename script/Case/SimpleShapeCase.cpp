#include "SimpleShapeCase.h"
#include "SimUtil.h"

SimpleShapeCase::SimpleShapeCase() :
    SimCase() {}

SimpleShapeCase::~SimpleShapeCase() {}

int SimpleShapeCase::getScreenWidth() const {
	return 1280;
};

int SimpleShapeCase::getScreenHeight() const {
	return 720;
};

void SimpleShapeCase::recycleEntity(std::string uuid) {
    expiredEntities.emplace_back(uuid);
}

void SimpleShapeCase::doUpdate(ComPtr<ID3D11Device> dev, double simTime, double frameTime) {
    // remove expired entities
    for (auto uuid : expiredEntities) {
        removeEntity(uuid);
    }
    expiredEntities.clear();
    // add entities randomly
    double p = uniformRandom();
    if (p < 0.1) {
        // addEntity(std::make_shared<FadingTriangle>(), dev);
    }
}