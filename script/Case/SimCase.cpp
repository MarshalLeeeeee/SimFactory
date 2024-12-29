#include "SimCase.h"

SimCase::SimCase() : pUI(nullptr) {
	startTime = std::chrono::high_resolution_clock::now();
	updateTime = startTime;
}

SimCase::~SimCase() {}

int SimCase::getScreenWidth() const {
	return 800;
};

int SimCase::getScreenHeight() const {
	return 600;
};

bool SimCase::init(HWND hWindow, ComPtr<ID3D11Device> dev, ComPtr<ID3D11DeviceContext> devCon) {
	if (!initUI(hWindow, dev, devCon)) return false;
	return true;
}

bool SimCase::needUI() const {
	return false;
}

bool SimCase::initUI(HWND hWindow, ComPtr<ID3D11Device> dev, ComPtr<ID3D11DeviceContext> devCon) {
	if (!needUI()) return true;
	pUI = std::make_shared<UI>(hWindow, dev, devCon);
	return true;
}

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

void SimCase::preRender(ComPtr<ID3D11DeviceContext> devCon) const {
	if (needUI()) {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
}

void SimCase::render(ComPtr<ID3D11DeviceContext> devCon) const {
	for (auto itr = renderObjs.begin(); itr != renderObjs.end(); ++itr) {
		itr->second->render(devCon);
	}
	if (needUI()) pUI->render();
}

bool SimCase::addEntity(std::shared_ptr<SimEntity> pSimEntity, ComPtr<ID3D11Device> dev) {
	if (!pSimEntity->init(dev)) return false;
	std::string uuid = pSimEntity->getUUID();
	if (hasEntity(uuid)) return false;
	entities[uuid] = pSimEntity;
	return true;
}

bool SimCase::removeEntity(std::string uuid) {
	if (hasEntity(uuid)) {
		entities.erase(uuid);
		return true;
	}
	else {
		return false;
	}
}

bool SimCase::hasEntity(std::string uuid) const {
	return entities.find(uuid) != entities.end();
}

std::shared_ptr<SimEntity> SimCase::getEntity(std::string uuid) const {
	if (!hasEntity(uuid)) return nullptr;
	else return entities.at(uuid);
}

bool SimCase::addRenderObj(std::shared_ptr<RenderObjBase> pRenderObj, ComPtr<ID3D11Device> dev) {
	std::string uuid = pRenderObj->getUUID();
	if (hasRenderObj(uuid)) return false;
	if (!pRenderObj->init(dev)) return false;
	renderObjs[uuid] = pRenderObj;
	return true;
}

bool SimCase::removeRenderObj(std::string uuid) {
	if (hasRenderObj(uuid)) {
		renderObjs.erase(uuid);
		return true;
	}
	else {
		return false;
	}
}

bool SimCase::hasRenderObj(std::string uuid) const {
	return renderObjs.find(uuid) != renderObjs.end();
}

std::shared_ptr<RenderObjBase> SimCase::getRenderObj(std::string uuid) const {
	if (!hasRenderObj(uuid)) return nullptr;
	else return renderObjs.at(uuid);
}

LRESULT CALLBACK SimCase::preProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (!needUI()) return 0;
	else if (pUI) {
		return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
	}
	else return 0;
}