#include "FadingTriangleCase.h"
#include "SimUtil.h"
#include "TypeUtil.h"
#include "LogUtil.h"
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
		addEntity(std::make_shared<FadingTriangle>(), dev);
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
	SimEntity(), c(0.0f), d(1.0f) {}

FadingTriangle::~FadingTriangle() {}

bool FadingTriangle::initRenderEntity(SimCase* pSimCase, ComPtr<ID3D11Device> dev) {
	VertexPosColor vData[3] = {
		{ DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f) , DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.45f, -0.5, 0.0f) , DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.45f, -0.5f, 0.0f) , DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }
	};
	pRenderEntity = std::make_unique<RenderEntity<VertexPosColor>>(
		uuid,
		vData,
		3
	);
	D3D_PRIMITIVE_TOPOLOGY primitiveTopologyData[1] = {
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	};
	DWORD iData[3] = {
		0, 1, 2
	};
	DWORD* indicesData[1] = {
		iData
	};
	uint32_t indicesLenData[1] = {
		3
	};
	bool res = pRenderEntity->init(
		pSimCase,
		dev,
		primitiveTopologyData,
		indicesData,
		indicesLenData,
		1
	);
	if (!res) pRenderEntity = nullptr;
	return res;
}

void FadingTriangle::updateProperty(SimCase* pSimCase, double simTime, double frameTime) {
	float period = static_cast<FadingTriangleCase*>(pSimCase)->getPeriod();
	c += d * float(frameTime) / period;
	if (c > 1.0f) {
		d = -1.0f;
		c = 2.0f - c;
	}
	else if (c < 0.0f) {
		d = 1.0f;
		c = -c;
	}
}

void FadingTriangle::updateRenderEntity() {
	if (!pRenderEntity) return;
	DirectX::XMFLOAT4 cs[3] = {
		{ DirectX::XMFLOAT4(c, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.0f, c, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.0f, 0.0f, c, 1.0f) },
	};
	for (uint32_t i = 0; i < 3; ++i) {
		pRenderEntity->updateField(i, "color", cs[i]);
	}
}



FadingTriangleControlPanel::FadingTriangleControlPanel(std::string name, FadingTriangleCase* pSimCase) :
	UIPanel(name) {
		addUIWidget(std::make_shared<UISliderFloat>(
			"PeriodSlider",
			1.0,
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
