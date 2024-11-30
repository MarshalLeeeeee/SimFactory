#include "SimApp.h"
#include "FadingTriangleCase.h"

SimApp::SimApp(HINSTANCE hInstance, int nCmdShow, int w, int h, const WCHAR* vsHLSL, const WCHAR* psHLSL) :
	DxApp(hInstance, nCmdShow, w, h, vsHLSL, psHLSL) {}

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
	pSimCase = std::make_shared<FadingTriangleCase>();
	return true;
}
