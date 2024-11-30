#include "SimApp.h"

SimApp::SimApp(HINSTANCE hInstance, int nCmdShow, int w, int h, const WCHAR* vsHLSL, const WCHAR* psHLSL) :
	DxApp(hInstance, nCmdShow, w, h, vsHLSL, psHLSL), simWorld() {}

SimApp::~SimApp() {}

void SimApp::update() {
	simWorld.update(dev);
}

void SimApp::render() {
	float bg[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	devCon->ClearRenderTargetView(rt.Get(), bg);
	simWorld.render(devCon);
	swChain->Present(0, 0);
}
