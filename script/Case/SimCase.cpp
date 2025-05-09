#include "SimCase.h"
#include "SimUtil.h"
#include "LogUtil.h"
#include "ThreadUtil.h"

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

bool SimCase::preInit() {
	return true;
}

bool SimCase::init(HWND hWindow, ComPtr<ID3D11Device> dev, ComPtr<ID3D11DeviceContext> devCon) {
	if (!initUI(hWindow, dev, devCon)) return false;
	width2height = static_cast<float>(getScreenWidth()) / static_cast<float>(getScreenHeight());
	return true;
}

LRESULT CALLBACK SimCase::simProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (pUI) return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
	else return 0;
}

bool SimCase::initUI(HWND hWindow, ComPtr<ID3D11Device> dev, ComPtr<ID3D11DeviceContext> devCon) {
	if (needUI()) pUI = std::make_unique<UI>(hWindow, dev, devCon);
	return true;
}

bool SimCase::needUI() const {
	return false;
}

void SimCase::update(ComPtr<ID3D11Device> dev) {
	MainthreadCallbackHub::getInstance().invoke();
	auto currTime = std::chrono::high_resolution_clock::now();
    double simTime = static_cast<std::chrono::duration<double>>(currTime - startTime).count();
    double frameTime = static_cast<std::chrono::duration<double>>(currTime - updateTime).count();
	doUpdate(dev, simTime, frameTime);
	updateTime = currTime;
}

void SimCase::doUpdate(ComPtr<ID3D11Device> dev, double simTime, double frameTime) {}

void SimCase::preRender(ComPtr<ID3D11Device> dev, HWND hWindow) {
	if (needUI()) {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
}

void SimCase::render(ComPtr<ID3D11DeviceContext> devCon) const {
	for (auto itr = entities.begin(); itr != entities.end(); ++itr) {
		itr->second->render(devCon);
	}
	if (pUI) pUI->render();
}

void SimCase::postRender(ComPtr<ID3D11Device> dev, HWND hWindow) {}

bool SimCase::addEntity(ComPtr<ID3D11Device> dev, std::shared_ptr<Entity> pSimEntity) {
	std::string uuid = generateUUID();
	if (hasEntity(uuid)) {
		Logger::getInstance().error("[SimCase::addEntity] duplicate uuid...");
		return false;
	}
	if (!pSimEntity->init(dev, this, uuid)) {
		Logger::getInstance().error("[SimCase::addEntity] fail to init sim entity...");
		return false;
	}
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

std::shared_ptr<Entity> SimCase::getEntity(std::string uuid) const {
	if (!hasEntity(uuid)) return nullptr;
	else return entities.at(uuid);
}

void SimCase::updateEntities(double simTime, double frameTime) {
	for (auto itr = entities.begin(); itr != entities.end(); ++itr) {
		itr->second->update(this, simTime, frameTime);
	}
}
