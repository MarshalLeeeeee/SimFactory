#include "SimEntity.h"
#include "SimUtil.h"
#include "LogUtil.h"

SimEntity::SimEntity() : 
	pModel(nullptr), 
	posX(0.f), posY(0.f), angle(0.f),
	scaleX(1.f), scaleY(1.f) {}

SimEntity::SimEntity(float posX_, float poxY_, float angle_, float scaleX_, float scaleY_) :
	pModel(nullptr), 
	posX(posX_), posY(poxY_), angle(angle_),
	scaleX(scaleX_), scaleY(scaleY_) {}

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
    updateModel(pSimCase);
}

void SimEntity::updateModel(SimCase* pSimCase) {
	if (pModel && pModel->getAwaken()) {
		if (!pModel->getLoaded()) {
			Logger::getInstance().error("[SimEntity::updateModel] model is not loaded...");
			pModel = nullptr;
		}
		else {
			doUpdateModel(pSimCase);
		}
	}
}

void SimEntity::doUpdateModel(SimCase* pSimCase) {}

std::string SimEntity::getUUID() const {
	return uuid;
}

void SimEntity::render(ComPtr<ID3D11DeviceContext> devCon) const {
	if (!pModel) return;
	if (!pModel->getLoaded()) return;
	pModel->render(devCon);
}

float SimEntity::getPosX() const {
	return posX;
}

float SimEntity::getPosY() const {
	return posY;
}

float SimEntity::getAngle() const {
	return angle;
}

float SimEntity::getScaleX() const {
	return scaleX;
}

float SimEntity::getScaleY() const {
	return scaleY;
}

void SimEntity::setPosX(float posX_){
	posX = posX_;
}

void SimEntity::setPosY(float posY_){
	posY = posY_;
}

void SimEntity::setAngle(float angle_){
	angle = angle_;
}

void SimEntity::setScaleX(float scaleX_){
	scaleX = scaleX_;
}

void SimEntity::setScaleY(float scaleY_){
	scaleY = scaleY_;
}