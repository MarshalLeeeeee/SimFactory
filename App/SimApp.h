#pragma once

#ifndef __SIMAPP_H_
#define __SIMAPP_H_

#include "DxApp.h"
#include "SimWorld.h"

class SimApp : public DxApp {
public:
	SimApp(HINSTANCE hInstance, int nCmdShow, int w, int h, const WCHAR* vsHLSL, const WCHAR* psHLSL);
	virtual ~SimApp();

	void update();
	void render();

private:
	SimWorld simWorld;
};

#endif
