#pragma once

#ifndef __SIMAPP_H_
#define __SIMAPP_H_

#include "DxApp.h"
#include "SimCase.h"
#include <memory>

class SimApp : public DxApp {
public:
	SimApp(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow);
	virtual ~SimApp();

	void update();
	void preRender() const;
	void render() const;

protected:
	bool initApp();
	bool initSim();
	LRESULT CALLBACK preProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	std::shared_ptr<SimCase> pSimCase;
};

#endif
