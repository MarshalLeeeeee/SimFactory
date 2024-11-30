#pragma once

#ifndef __DXAPP_H__
#define __DXAPP_H__

#include <Windows.h>
#include <wrl/client.h>

// #include <DirectXMath.h>
// #include <DirectXPackedVector.h>
// #include <DirectXColors.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
// #pragma comment(lib, "dxguid.lib")
// #pragma comment(lib, "winmm.lib")

#include <string>

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

class DxApp {
public:
	DxApp(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow, int w, int h, const WCHAR* vsHLSL, const WCHAR* psHLSL);
	virtual ~DxApp();

	int run();
	virtual bool init();
	virtual void update() = 0;
	virtual void render() = 0;
	LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	bool initWindow();
	bool initDx();
	bool initPipeline();
	virtual bool initApp() = 0;

	ComPtr<ID3D11Device> dev;
	ComPtr<ID3D11DeviceContext> devCon;
	ComPtr<IDXGISwapChain> swChain;
	ComPtr<ID3D11RenderTargetView> rt;
	ComPtr<ID3D11InputLayout> vLayout;
	ComPtr<ID3D11VertexShader> vs;
	ComPtr<ID3D11PixelShader> ps;

	HINSTANCE hInstance;
	LPSTR lpCmdLine;
	int nCmdShow;
	HWND hWindow;
	int w; // width of window
	int h; // height of window
	std::wstring vsHLSL;
	std::wstring psHLSL;
};

#endif
