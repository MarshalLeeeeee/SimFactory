#include "Entity.h"
#include "SimUtil.h"
#include "LogUtil.h"

Entity::Entity() : 
	pModel(nullptr), 
	posX(0.f), posY(0.f), angle(0.f),
	scaleX(1.f), scaleY(1.f) {}

Entity::Entity(float posX_, float poxY_, float angle_, float scaleX_, float scaleY_) :
	pModel(nullptr), 
	posX(posX_), posY(poxY_), angle(angle_),
	scaleX(scaleX_), scaleY(scaleY_) {}

Entity::~Entity() {}

bool Entity::init(ComPtr<ID3D11Device> dev, SimCase* pSimCase, std::string uuid) {
	if (!initEntity()) return false;
	this->uuid = uuid;
	initModel(dev, pSimCase);
	return true;
}

void Entity::initModel(ComPtr<ID3D11Device> dev, SimCase* pSimCase) {}

bool Entity::initEntity() {
	return true;
}

void Entity::update(SimCase* pSimCase, double simTime, double frameTime) {
	updateProperty(pSimCase, simTime, frameTime);
    updateModel(pSimCase);
}

void Entity::updateModel(SimCase* pSimCase) {
	if (pModel && pModel->getAwaken()) {
		if (!pModel->getLoaded()) {
			Logger::getInstance().error("[Entity::updateModel] model is not loaded...");
			pModel = nullptr;
		}
		else {
			doUpdateModel(pSimCase);
		}
	}
}

void Entity::doUpdateModel(SimCase* pSimCase) {}

std::string Entity::getUUID() const {
	return uuid;
}

void Entity::render(ComPtr<ID3D11DeviceContext> devCon) const {
	if (!pModel) return;
	if (!pModel->getLoaded()) return;
	pModel->render(devCon);
}

float Entity::getPosX() const {
	return posX;
}

float Entity::getPosY() const {
	return posY;
}

float Entity::getAngle() const {
	return angle;
}

float Entity::getScaleX() const {
	return scaleX;
}

float Entity::getScaleY() const {
	return scaleY;
}

void Entity::setPosX(float posX_){
	posX = posX_;
}

void Entity::setPosY(float posY_){
	posY = posY_;
}

void Entity::setAngle(float angle_){
	angle = angle_;
}

void Entity::setScaleX(float scaleX_){
	scaleX = scaleX_;
}

void Entity::setScaleY(float scaleY_){
	scaleY = scaleY_;
}