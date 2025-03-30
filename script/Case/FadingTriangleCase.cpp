#include "FadingTriangleCase.h"
#include "SimUtil.h"
#include "TypeUtil.h"
#include "LogUtil.h"
#include "MathUtil.h"
#include "UIPanel.h"
#include "UIWidget.h"

FadingTriangleCase::FadingTriangleCase() :
    SimCase(), period(1.0f), refreshTime(-99.0)
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

	if (simTime > refreshTime + 1.0) {
		float posX = static_cast<float>(uniformRandom() * 2.0 - 1.0);
		float posY = static_cast<float>(uniformRandom() * 2.0 - 1.0);
		float angle = static_cast<float>(uniformRandom() * PI_2);
		float scale = static_cast<float>(uniformRandom());
		addEntity(dev, std::make_shared<FadingTriangle>(
			posX,
			posY,
			angle,
			scale,
			scale,
			simTime
		));
		refreshTime = simTime;
	}
	std::vector<std::string> expiredUUID;
	for (auto itr = entities.begin(); itr != entities.end(); ++itr) {
		std::shared_ptr<FadingTriangle> pFadingTriangle = std::dynamic_pointer_cast<FadingTriangle>(itr->second);
		if (pFadingTriangle->expired(simTime)) {
			expiredUUID.emplace_back(itr->first);
		}
	}
	for (std::string& uuid : expiredUUID) {
		removeEntity(uuid);
	}
	updateEntities(simTime, frameTime);
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



FadingTriangle::FadingTriangle(double simTime) :
	SimEntity(), opacity(1.f), expireTime(simTime+LIFESPAN) {}

FadingTriangle::FadingTriangle(float posX, float posY, float angle, float scaleX, float scaleY, double simTime) :
	SimEntity(posX, posY, angle, scaleX, scaleY), opacity(1.f), expireTime(simTime+LIFESPAN) {}

FadingTriangle::~FadingTriangle() {}

bool FadingTriangle::expired(double simTime) const {
	return simTime > expireTime;
}

void FadingTriangle::initModel(ComPtr<ID3D11Device> dev, SimCase* pSimCase) {
	pModel = std::make_unique<Model>(dev, "triangle");
}

void FadingTriangle::updateProperty(SimCase* pSimCase, double simTime, double frameTime) {
	float period = static_cast<FadingTriangleCase*>(pSimCase)->getPeriod();
	opacity -= float(1.0 / LIFESPAN) * float(frameTime);
	if (opacity < 0.f) opacity = 0.f;
	if (opacity < 0.1) Logger::getInstance().info("FadingTriangle opacity low: ", opacity);
	angle += float(frameTime) * period;
	if (angle > PI_2) angle -= PI_2;
    else if (angle < 0.) angle += PI_2;
}

void FadingTriangle::doUpdateModel(SimCase* pSimCase) {
	pModel->updateTransformData(getTransformMatrix(posX, posY, angle, scaleX, scaleY, pSimCase->width2height));
	Any any(DirectX::XMFLOAT4(opacity, opacity, 0.0f, 0.0f));
	pModel->updateMeshPixelMaterialBuffer("triangle_main", "pixelbuffer", any);
}



FadingTriangleControlPanel::FadingTriangleControlPanel(std::string name, FadingTriangleCase* pSimCase) :
	UIPanel(name) {
		addUIWidget(std::make_shared<UISliderFloat>(
			"SpeedSlider",
			0.0,
			5.0,
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
