#include "SimApp.h"
#include "SimUtil.h"
#include "FadingTriangleCase.h"

SimApp::SimApp(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow) :
	DxApp(hInstance, lpCmdLine, nCmdShow) {}

SimApp::~SimApp() {}

void SimApp::update() {
	pSimCase->update(dev);
}

void SimApp::render() {
	float bg[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	devCon->ClearRenderTargetView(rt.Get(), bg);
	pSimCase->render(devCon);
	swChain->Present(0, 0);
}

bool SimApp::initApp() {
	pSimCase = createSimCase(cmdArgs);
	w = pSimCase->getScreenWidth();
	h = pSimCase->getScreenHeight();
	vsHLSL = std::wstring(pSimCase->getVsHLSL());
	psHLSL = std::wstring(pSimCase->getPsHLSL());
	return true;
}
