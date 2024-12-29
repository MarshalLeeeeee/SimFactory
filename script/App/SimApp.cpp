#include "SimApp.h"
#include "SimUtil.h"

#include "SimCase.h"
#include "FadingTriangleCase.h"

std::shared_ptr<SimCase> createSimCase(const std::vector<std::string>& cmdArgs) {
    if (cmdArgs.empty()) return std::make_shared<SimCase>();
    else {
        std::string caseName = cmdArgs[0];
        if (caseName == "FadingTriangleCase") return std::make_shared<FadingTriangleCase>();
        else return std::make_shared<SimCase>();
    }
}

SimApp::SimApp(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow) :
	DxApp(hInstance, lpCmdLine, nCmdShow) {}

SimApp::~SimApp() {}

bool SimApp::initApp() {
	pSimCase = createSimCase(cmdArgs);
	w = pSimCase->getScreenWidth();
	h = pSimCase->getScreenHeight();
	return true;
}

bool SimApp::initFunc() {
	return pSimCase->init(hWindow, dev, devCon);
}

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

void SimApp::postRender() const {}

LRESULT CALLBACK SimApp::appProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (pSimCase) {
		return pSimCase->preProc(hwnd, msg, wParam, lParam);
	}
	else return 0;
}
