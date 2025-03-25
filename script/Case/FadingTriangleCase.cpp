#include "FadingTriangleCase.h"
#include "SimUtil.h"
#include "TypeUtil.h"
#include "LogUtil.h"
#include "MathUtil.h"
#include "UIPanel.h"
#include "UIWidget.h"

FadingTriangleCase::FadingTriangleCase() :
    SimCase(), period(1.0f)
#ifndef NDEBUG
	, showDebugPnl(false)
	, renderDocApi(nullptr), captureFrameSwitch(false), captureFraming(false) 
#endif
	{}

FadingTriangleCase::~FadingTriangleCase() {}

int FadingTriangleCase::getScreenWidth() const {
	return 1280;
}

int FadingTriangleCase::getScreenHeight() const {
	return 960;
}

bool FadingTriangleCase::needUI() const {
	return true;
}

bool FadingTriangleCase::preInit() {
	return initRenderDoc();
}

void FadingTriangleCase::setPeriod(float p) {
	period = p;
}

float FadingTriangleCase::getPeriod() const {
	return period;
}

#ifndef NDEBUG
void FadingTriangleCase::setShowDebugPnl(bool b) {
	showDebugPnl = b;
}
#endif

#ifndef NDEBUG
void FadingTriangleCase::enableCaptureFrame() {
	captureFrameSwitch = true;
}
#endif

bool FadingTriangleCase::initRenderDoc() {
#ifndef NDEBUG
	HMODULE mod = LoadLibraryA("renderdoc.dll");
	if (mod == NULL) {
		MessageBox(0, L"Load renderdoc.dll failed...", 0, 0);
		Logger::getInstance().error("[FadingTriangleCase::initRenderDoc] Load renderdoc.dll failed...");
		return false;
	}
	
	pRENDERDOC_GetAPI RENDERDOC_GetAPI = (pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
	int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_6_0, (void**)&renderDocApi);
	if (ret != 1) {
		MessageBox(0, L"Init renderdoc api failed...", 0, 0);
		Logger::getInstance().error("[FadingTriangleCase::initRenderDoc] Init renderdoc api failed...");
		return false;
	}
	if (!renderDocApi) {
		MessageBox(0, L"Init renderdoc api failed...", 0, 0);
		Logger::getInstance().error("[FadingTriangleCase::initRenderDoc] Init renderdoc api failed...");
		return false;
	}
	renderDocApi->SetCaptureFilePathTemplate("captures/frame");
#endif
	return true;
}

void FadingTriangleCase::doUpdate(ComPtr<ID3D11Device> dev, double simTime, double frameTime) {
	// update ui and binding value
	if (!pUI->hasUIPanel("Control")) {
		pUI->addUIPanel(std::make_shared<FadingTriangleControlPanel>("Control", this));
	}

#ifndef NDEBUG
	if (showDebugPnl) {
		pUI->addUIPanel(std::make_shared<FadingTriangleDebugPanel>("Debug", this));
	}
	else {
		pUI->removeUIPanel("Debug");
	}
#endif

	if (entities.empty()) {
		addEntity(dev, std::make_shared<FadingTriangle>());
	}
	else {
		updateEntities(simTime, frameTime);
	}
}

void FadingTriangleCase::preRender(ComPtr<ID3D11Device> dev, HWND hWindow) {
	SimCase::preRender(dev, hWindow);
#ifndef NDEBUG
	if (captureFrameSwitch) {
		renderDocApi->StartFrameCapture(NULL, NULL);
		captureFraming = true;
		captureFrameSwitch = false;
		Logger::getInstance().debug("Start frame capture...");
	}
#endif
}

void FadingTriangleCase::postRender(ComPtr<ID3D11Device> dev, HWND hWindow) {
#ifndef NDEBUG
	if (captureFraming) {
		renderDocApi->EndFrameCapture(NULL, NULL);
		captureFraming = false;
		Logger::getInstance().debug("End frame capture...");
	}
#endif
	SimCase::postRender(dev, hWindow);
}



FadingTriangle::FadingTriangle() :
	SimEntity(), co(1.f), d(1.f), a(0.f) {}

FadingTriangle::~FadingTriangle() {}

void FadingTriangle::initModel(ComPtr<ID3D11Device> dev, SimCase* pSimCase) {
	pModel = std::make_unique<Model>(dev, "triangle");
}

void FadingTriangle::updateProperty(SimCase* pSimCase, double simTime, double frameTime) {
	float period = static_cast<FadingTriangleCase*>(pSimCase)->getPeriod();
	co += d * float(frameTime) * period;
	if (co > 1.0f) {
		d = -1.0f;
		co = 2.0f - co;
	}
	else if (co < 0.0f) {
		d = 1.0f;
		co = -co;
	}
	a += float(frameTime) * period;
	if (a > PI_2) a -= PI_2;
    else if (a < 0.) a += PI_2;
}

void FadingTriangle::updateModel() {
	if (!pModel) return;
}



FadingTriangleControlPanel::FadingTriangleControlPanel(std::string name, FadingTriangleCase* pSimCase) :
	UIPanel(name) {
		addUIWidget(std::make_shared<UISliderFloat>(
			"PeriodSlider",
			0.0,
			10.0,
			std::bind(&FadingTriangleCase::setPeriod, pSimCase, std::placeholders::_1)
		));
#ifndef NDEBUG
		addUIWidget(std::make_shared<UICheckbox>(
			"DebugChecker",
			std::bind(&FadingTriangleCase::setShowDebugPnl, pSimCase, std::placeholders::_1)
		));
#endif
	}
FadingTriangleControlPanel::~FadingTriangleControlPanel() {}


FadingTriangleDebugPanel::FadingTriangleDebugPanel(std::string name, FadingTriangleCase* pSimCase) :
	UIPanel(name) {
#ifndef NDEBUG
		addUIWidget(std::make_shared<UIButton>(
			"FrameCapture",
			std::bind(&FadingTriangleCase::enableCaptureFrame, pSimCase)
		));
#endif
	}
FadingTriangleDebugPanel::~FadingTriangleDebugPanel() {}
