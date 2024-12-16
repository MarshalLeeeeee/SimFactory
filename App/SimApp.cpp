#include "SimApp.h"
#include "SimUtil.h"

SimApp::SimApp(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow) :
	DxApp(hInstance, lpCmdLine, nCmdShow) {}

SimApp::~SimApp() {}

void SimApp::update() {
	pSimCase->update(dev);
}

void SimApp::preRender() const {
	pSimCase->preRender(devCon);
}

void SimApp::render() const {
	float bg[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	devCon->ClearRenderTargetView(rt.Get(), bg);
	pSimCase->render(devCon);
	swChain->Present(0, 0);
}

bool SimApp::initApp() {
	pSimCase = createSimCase(cmdArgs);
	w = pSimCase->getScreenWidth();
	h = pSimCase->getScreenHeight();
	return true;
}

bool SimApp::initSim() {
	return pSimCase->init(hWindow, dev, devCon);
}

LRESULT CALLBACK SimApp::preProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (pSimCase) {
		return pSimCase->preProc(hwnd, msg, wParam, lParam);
	}
	else return 0;
}
