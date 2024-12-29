/*
 * SimApp
 * hold sim case, which represents simulation logic and rendering
 */
#pragma once

#ifndef __SIMAPP_H_
#define __SIMAPP_H_

#include "DxApp.h"
#include "SimCase.h"

#include <memory>

/*
* SimCase factory
* Return specific SimCase given the cmd args
*/
std::shared_ptr<SimCase> createSimCase(const std::vector<std::string>& cmdArgs);

class SimApp : public DxApp {
public:
	SimApp(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow);
	virtual ~SimApp();

protected:
	/* The first initialization step, initialize application constants */
	bool initApp();
	/* The last initialization step, initialize application logic */
	bool initFunc();

protected:
	/* Logic update */
	void update();
	/* Pre stage of rendering */
	void preRender() const;
	/* Rendering */
	void render() const;
	/* Post stage of rendering */
	void postRender() const;
	/* Application event handler */
	LRESULT CALLBACK appProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	/* ptr of specific sim case */
	std::shared_ptr<SimCase> pSimCase;
};

#endif
