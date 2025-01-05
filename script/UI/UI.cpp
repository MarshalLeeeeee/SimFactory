#include "UI.h"

UI::UI(SimCase* pSimCase, HWND hWindow, ComPtr<ID3D11Device> dev, ComPtr<ID3D11DeviceContext> devCon) :
    pSimCase(pSimCase) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(hWindow);
    ImGui_ImplDX11_Init(dev.Get(), devCon.Get());
}

UI::~UI() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void UI::render() const {
    for (auto itr = uiPanels.begin(); itr != uiPanels.end(); ++itr) {
        ImGui::Begin(itr->first.c_str());
        itr->second->render();
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

bool UI::addUIPanel(std::shared_ptr<UIPanel> pUIPanel) {
    std::string name = pUIPanel->getName();
    if (hasUIPanel(name)) {
        return false;
    }
    else {
        uiPanels[name] = pUIPanel;
        return true;
    }
}

bool UI::removeUIPanel(std::string name) {
    if (hasUIPanel(name)) {
        uiPanels.erase(name);
        return true;
    }
    else {
        return false;
    }
}
bool UI::hasUIPanel(std::string name) const {
    return uiPanels.find(name) != uiPanels.end();
}

std::shared_ptr<UIPanel> UI::getUIPanel(std::string name) const {
    if (hasUIPanel(name)) {
        return uiPanels.at(name);
    }
    else return nullptr;
}
