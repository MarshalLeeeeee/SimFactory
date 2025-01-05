/*
 * UI
 * A singleton manager
 */

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

#include "SimCase.h"

class UIPanel;

class UI {
public:
    UI(SimCase* pSimCase, HWND hWindow, ComPtr<ID3D11Device> dev, ComPtr<ID3D11DeviceContext> devCon);
    virtual ~UI();
    /* render the whole interface including all panels */
    void render() const;
private:
    SimCase* pSimCase;

public:
    bool addUIPanel(std::shared_ptr<UIPanel> pUIPanel);
    bool removeUIPanel(std::string name);
    bool hasUIPanel(std::string name) const;
    std::shared_ptr<UIPanel> getUIPanel(std::string name) const;
private:
    /* all panels with exclusive names as key */
    std::unordered_map<std::string, std::shared_ptr<UIPanel>> uiPanels;

};

#include "UIPanel.h"

#endif
