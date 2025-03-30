#include "DxApp.h"
#include "GraphicsUtil.h"
#include "SimUtil.h"
#include "LogUtil.h"

namespace {
	DxApp* thisptr = nullptr;
}

LRESULT CALLBACK mainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return thisptr->wndProc(hwnd, msg, wParam, lParam);
}

DxApp::DxApp(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow) :
	dev(nullptr), devCon(nullptr),
	swChain(nullptr), rt(nullptr),
	hInstance(hInstance), 
	nCmdShow(nCmdShow), hWindow(nullptr) {
	thisptr = this;
	unparseLpCmdLine(lpCmdLine, cmdArgs);
}

DxApp::~DxApp() {}

bool DxApp::init() {
	if (!initApp()) return false;
	if (!initWindow()) return false;
	if (!initDx()) return false;
	if (!initCase()) return false;
	Logger::getInstance().info("App is successfully initialized...");
	return true;
}

bool DxApp::initWindow() {
	// Register window class
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = mainWndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = L"DxAppWC";
	if (!RegisterClassEx(&wc)) {
		MessageBox(0, L"Register window class failed...", 0, 0);
		Logger::getInstance().error("[DxApp::initWindow] Register window class failed...");
		return false;
	}

	// Create window handle
	RECT wr = { 0, 0, w, h };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
	hWindow = CreateWindowEx(
		NULL,
		L"DxAppWC",
		L"App",
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (!hWindow) {
		MessageBox(0, L"Create window failed...", 0, 0);
		Logger::getInstance().error("[DxApp::initWindow] Create window handler failed...");
		return false;
	}
	ShowWindow(hWindow, nCmdShow);
	UpdateWindow(hWindow);

	return true;
}

bool DxApp::initDx() {
	// Create swap chain
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1; // one back buffer
	scd.BufferDesc.Width = w; // set the back buffer width
	scd.BufferDesc.Height = h; // set the back buffer height
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // how swap chain is to be used
	scd.OutputWindow = hWindow; // the window to be used
	scd.SampleDesc.Count = 4; // how many multisamples
	scd.Windowed = TRUE; // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // allow full-screen switching
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		featureLevels,
		numFeatureLevels,
		D3D11_SDK_VERSION,
		&scd,
		swChain.GetAddressOf(),
		dev.GetAddressOf(),
		NULL,
		devCon.GetAddressOf()
	);
	if (FAILED(hr)) {
		MessageBox(0, L"CreateDeviceAndSwapChain failed...", 0, 0);
		Logger::getInstance().error("[DxApp::initDx] CreateDeviceAndSwapChain failed...");
		return false;
	}

	// Create RT
	ComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
	hr = swChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)pBackBuffer.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(0, L"Get back buffer failed...", 0, 0);
		Logger::getInstance().error("[DxApp::initDx] Get back buffer failed...");
		return false;
	}
	hr = dev->CreateRenderTargetView(pBackBuffer.Get(), NULL, rt.GetAddressOf()); // use the back buffer address to create the render target
	pBackBuffer->Release();
	if (FAILED(hr)) {
		MessageBox(0, L"Create render target failed...", 0, 0);
		Logger::getInstance().error("[DxApp::initDx] Create render target failed...");
		return false;
	}
	devCon->OMSetRenderTargets(1, rt.GetAddressOf(), nullptr); // set the render target as the back buffer

	// Set viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = w;
	viewport.Height = h;
	devCon->RSSetViewports(1, &viewport);

	return true;
}

int DxApp::run() {
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	Logger::getInstance().info("App starts...");
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		update();
		preRender();
		render();
		postRender();
	}
	Logger::getInstance().info("App ends...");
	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK DxApp::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (appProc(hwnd, msg, wParam, lParam)) return true;

	switch (msg) {
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
