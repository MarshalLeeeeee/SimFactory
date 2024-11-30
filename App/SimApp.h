#pragma once

#ifndef __SIMAPP_H_
#define __SIMAPP_H_

#include "DxApp.h"
#include "SimCase.h"
#include <memory>

class SimApp : public DxApp {
public:
	SimApp(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow, int w, int h, const WCHAR* vsHLSL, const WCHAR* psHLSL);
	virtual ~SimApp();

	void update();
	void render();

protected:
	bool initApp();

	std::shared_ptr<SimCase> pSimCase;
};

#endif
