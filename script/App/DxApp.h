/*
 * Basic DirectX frame irrelevant to simulation logic
 * Holding window and its config
 * Holding dx and its coms
 */

#pragma once

#ifndef __DXAPP_H__
#define __DXAPP_H__

#include <Windows.h>
#include <wrl/client.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <string>
#include <vector>

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

class DxApp {
public:
	DxApp(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow);
	virtual ~DxApp();

public:
	/*
	* The initialization of the application, only if the result is true can the application function correctly.
	* It is okay to throw error in these stages.
	*/
	bool init();
protected:
	/* The first initialization step, initialize application constants */
	virtual bool initApp() = 0;
	/* The window initialization */
	bool initWindow();
	/* The direct x initialization */
	bool initDx();
	/* The last initialization step, initialize application logic */
	virtual bool initCase() = 0;
protected:
	/* Standard hInstance from main entry */
	HINSTANCE hInstance;
	/* Unparsed cmd args from standard lpCmdLine from main entry */
	std::vector<std::string> cmdArgs;
	/* Standard nCmdShow from main entry */
	int nCmdShow;
	/* Width of window */
	int w;
	/* Height of window */
	int h;
	/* Window handler */
	HWND hWindow;
	/* Dx COMs */
	ComPtr<ID3D11Device> dev;
	ComPtr<ID3D11DeviceContext> devCon;
	ComPtr<IDXGISwapChain> swChain;
	ComPtr<ID3D11RenderTargetView> rt;

public:
	/*
	* The entry of the application.
	* A while loop with event listener and performs logic update and rendering.
	*/
	int run();
	/* Standard windows event handler */
	LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
protected:
	/* Logic update */
	virtual void update() = 0;
	/* Pre stage of rendering */
	virtual void preRender() const = 0;
	/* Rendering */
	virtual void render() const = 0;
	/* Post stage of rendering */
	virtual void postRender() const = 0;
	/* Application event handler */
	virtual LRESULT CALLBACK appProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};

#endif
