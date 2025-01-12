#include "FadingTriangleCase.h"
#include "SimUtil.h"
#include "TypeUtil.h"
#include "UIPanel.h"
#include "UIWidget.h"

FadingTriangleCase::FadingTriangleCase() :
    SimCase(), period(1.0f), show_debug_pnl(false),
	rdoc_api(nullptr), should_capture_frame(false) {}

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

bool FadingTriangleCase::initCase() {
    HMODULE mod = LoadLibraryA("renderdoc.dll");
    if (mod == NULL) {
        MessageBox(0, L"Load renderdoc.dll failed...", 0, 0);
		return false;
    }

    pRENDERDOC_GetAPI RENDERDOC_GetAPI = (pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
    int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_6_0, (void**)&rdoc_api);
    if (ret != 1) {
        MessageBox(0, L"Init renderdoc api failed...", 0, 0);
		return false;
    }

	return true;
}

void FadingTriangleCase::set_period(float p) {
	period = p;
}

void FadingTriangleCase::doUpdate(ComPtr<ID3D11Device> dev, double simTime, double frameTime) {
	// update ui and binding value
	std::shared_ptr<UIPanel> pControlPanel = pUI->getUIPanel("Control");
	if (pControlPanel) {
		std::shared_ptr<UIWidget> pPeriodSlider = pControlPanel->getUIWidget("PeriodSlider");
		if (pPeriodSlider) {
			// period = pPeriodSlider->getValue().get<float>();
		}
		else {
			pControlPanel->addUIWidget(std::make_shared<UISliderFloat>(
				this,
				"PeriodSlider",
				1.0,
				10.0,
				std::bind(&FadingTriangleCase::set_period, this, std::placeholders::_1)
			));
		}

		std::shared_ptr<UIWidget> pDebugChecker = pControlPanel->getUIWidget("DebugChecker");
		if (pDebugChecker) {
			show_debug_pnl = pDebugChecker->getValue().get<bool>();
		}
		else {
			pControlPanel->addUIWidget(std::make_shared<UICheckbox>(this, "DebugChecker"));
		}
	}
	else {
		pUI->addUIPanel(std::make_shared<FadingTriangleControlPanel>(this, "Control"));
	}

	if (show_debug_pnl) {
		pUI->addUIPanel(std::make_shared<FadingTriangleDebugPanel>(this, "Debug"));
	}
	else {
		pUI->removeUIPanel("Debug");
	}

	if (entities.empty()) {
		addEntity(std::make_shared<FadingTriangle>(this), dev);
	}
	else {
		updateEntities(simTime, frameTime);
	}
}

void FadingTriangleCase::preRender(ComPtr<ID3D11DeviceContext> devCon) {
	SimCase::preRender(devCon);
	if (should_capture_frame) {
		rdoc_api->StartFrameCapture(NULL, NULL);
	}
}

void FadingTriangleCase::postRender(ComPtr<ID3D11DeviceContext> devCon) {
	SimCase::postRender(devCon);
	if (should_capture_frame) {
		rdoc_api->EndFrameCapture(NULL, NULL);
		should_capture_frame = false;
	}
}



FadingTriangle::FadingTriangle(FadingTriangleCase* pSimCase) :
	SimEntity(pSimCase), c(0.0f), d(1.0f) {}

FadingTriangle::~FadingTriangle() {}

bool FadingTriangle::initRenderEntity(ComPtr<ID3D11Device> dev) {
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
	VertexPosColor vData[3] = {
		{ DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f) , DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.45f, -0.5, 0.0f) , DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.45f, -0.5f, 0.0f) , DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }
	};
	pRenderEntity = std::make_unique<RenderEntity<VertexPosColor>>(
		pSimCase,
		uuid,
		primitiveTopologyData,
		indicesData,
		indicesLenData,
		1,
		vData,
		3
	);
	bool res = pRenderEntity->init(dev);
	if (!res) pRenderEntity = nullptr;
	return res;
}

void FadingTriangle::updateProperty(double simTime, double frameTime) {
	float period = static_cast<FadingTriangleCase*>(pSimCase)->period;
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



FadingTriangleControlPanel::FadingTriangleControlPanel(SimCase* pSimCase, std::string name) :
	UIPanel(pSimCase, name) {
		// addUIWidget(std::make_shared<UISliderFloat>(pSimCase, "PeriodSlider", 1.0, 10.0));
		// addUIWidget(std::make_shared<UICheckbox>(pSimCase, "DebugChecker"));
	}
FadingTriangleControlPanel::~FadingTriangleControlPanel() {}


FadingTriangleDebugPanel::FadingTriangleDebugPanel(SimCase* pSimCase, std::string name) :
	UIPanel(pSimCase, name) {
		// addUIWidget(std::make_shared<UIButton>(pSimCase, "CaptureFrameBtn"));
	}
FadingTriangleDebugPanel::~FadingTriangleDebugPanel() {}
