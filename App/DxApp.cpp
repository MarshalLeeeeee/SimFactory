#include "DxApp.h"
#include "GraphicsUtil.h"

namespace {
	DxApp* thisptr = nullptr;
}

LRESULT CALLBACK mainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return thisptr->wndProc(hwnd, msg, wParam, lParam);
}

DxApp::DxApp(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow, int w, int h, const WCHAR* vsHLSL, const WCHAR* psHLSL) :
	dev(nullptr), devCon(nullptr),
	swChain(nullptr), rt(nullptr),
	hInstance(hInstance), lpCmdLine(lpCmdLine),
	nCmdShow(nCmdShow), hWindow(nullptr),
	w(w), h(h),
	vsHLSL(vsHLSL), psHLSL(psHLSL) {
	thisptr = this;
}

DxApp::~DxApp() {}

int DxApp::run() {
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		update();
		render();
	}
	return static_cast<int>(msg.wParam);
}

bool DxApp::init() {
	if (!initWindow()) return false;
	if (!initDx()) return false;
	if (!initPipeline()) return false;
	if (!initApp()) return false;
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
		return false;
	}

	// Create window handle
	RECT wr = { 0, 0, w, h };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
	hWindow = CreateWindowEx(
		NULL,
		L"DxAppWC",
		L"App",
		WS_OVERLAPPEDWINDOW,
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
		return false;
	}

	// Create RT
	ComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
	hr = swChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)pBackBuffer.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(0, L"Get back buffer failed...", 0, 0);
		return false;
	}
	hr = dev->CreateRenderTargetView(pBackBuffer.Get(), NULL, rt.GetAddressOf()); // use the back buffer address to create the render target
	pBackBuffer->Release();
	if (FAILED(hr)) {
		MessageBox(0, L"Create render target failed...", 0, 0);
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

bool DxApp::initPipeline() {
	ComPtr<ID3DBlob> blob;

	// Compile and create vertex shader
	if (FAILED(CreateShaderFromFile(vsHLSL.c_str(), "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()))) {
		MessageBox(0, L"Compile vertex shader failed...", 0, 0);
		return false;
	}
	if (FAILED(dev->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, vs.GetAddressOf()))) {
		MessageBox(0, L"Create vertex shader failed...", 0, 0);
		return false;
	}
	

	// Load vertex layout
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	if (FAILED(dev->CreateInputLayout(ied, ARRAYSIZE(ied),
		blob->GetBufferPointer(), blob->GetBufferSize(), vLayout.GetAddressOf()))) {
		MessageBox(0, L"Set input layout failed...", 0, 0);
		return false;
	}

	// Compile and create pixel shader
	if (FAILED(CreateShaderFromFile(psHLSL.c_str(), "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()))) {
		MessageBox(0, L"Compile pixel shader failed...", 0, 0);
		return false;
	}
	if (FAILED(dev->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, ps.GetAddressOf()))) {
		MessageBox(0, L"Create pixel shader failed...", 0, 0);
		return false;
	}

	// Set everything
	devCon->VSSetShader(vs.Get(), nullptr, 0);
	devCon->PSSetShader(ps.Get(), nullptr, 0);
	devCon->IASetInputLayout(vLayout.Get());
	devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

LRESULT CALLBACK DxApp::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
