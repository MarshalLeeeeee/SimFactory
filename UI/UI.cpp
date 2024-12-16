#include "UI.h"

UI::UI(HWND hWindow, ComPtr<ID3D11Device> dev, ComPtr<ID3D11DeviceContext> devCon) {
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
    {
        ImGui::Begin("Control Panel");
        for (auto itr = uiWidgets.begin(); itr != uiWidgets.end(); ++itr) {
            itr->second->render();
        }
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

bool UI::addUIWidget(std::shared_ptr<UIWidget> pUIWidget) {
    std::string uuid = pUIWidget->getUUID();
    if (hasUIWidget(uuid)) {
        return false;
    }
    else {
        uiWidgets[uuid] = pUIWidget;
        return true;
    }
}

bool UI::removeUIWidget(std::string uuid) {
    if (hasUIWidget(uuid)) {
        uiWidgets.erase(uuid);
        return true;
    }
    else {
        return false;
    }
}
bool UI::hasUIWidget(std::string uuid) const {
    return uiWidgets.find(uuid) != uiWidgets.end();
}

std::shared_ptr<UIWidget> UI::getUIWidget(std::string uuid) const {
    if (hasUIWidget(uuid)) {
        return uiWidgets.at(uuid);
    }
    else return nullptr;
}
