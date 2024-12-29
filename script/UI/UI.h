// A user interface based on ImGui
#pragma once

#ifndef __UI_H__
#define __UI_H__

#include <Windows.h>
#include <wrl/client.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#include <memory>
#include <string>
#include <unordered_map>

#include "UIWidget.h"

class UI {
public:
    UI(HWND hWindow, ComPtr<ID3D11Device> dev, ComPtr<ID3D11DeviceContext> devCon);
    virtual ~UI();

    void render() const;

    bool addUIWidget(std::shared_ptr<UIWidget> pUIWidget);
    bool removeUIWidget(std::string uuid);
    bool hasUIWidget(std::string uuid) const;
    std::shared_ptr<UIWidget> getUIWidget(std::string uuid) const;

private:

    std::unordered_map<std::string, std::shared_ptr<UIWidget>> uiWidgets;

};

#endif
