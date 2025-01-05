#include "UIPanel.h"

UIPanel::UIPanel(SimCase* pSimCase, std::string name) :
    pSimCase(pSimCase), name(name) {}

UIPanel::~UIPanel() {}

void UIPanel::render() const {
    for (auto itr = uiWidgets.begin(); itr != uiWidgets.end(); ++itr) {
        itr->second->render();
    }
}

std::string UIPanel::getName() const {
    return name;
}

bool UIPanel::addUIWidget(std::shared_ptr<UIWidget> pUIWidget) {
    std::string name = pUIWidget->getName();
    if (hasUIWidget(name)) {
        return false;
    }
    else {
        uiWidgets[name] = pUIWidget;
        return true;
    }
}

bool UIPanel::removeUIWidget(std::string name) {
    if (hasUIWidget(name)) {
        uiWidgets.erase(name);
        return true;
    }
    else {
        return false;
    }
}
bool UIPanel::hasUIWidget(std::string name) const {
    return uiWidgets.find(name) != uiWidgets.end();
}

std::shared_ptr<UIWidget> UIPanel::getUIWidget(std::string name) const {
    if (hasUIWidget(name)) {
        return uiWidgets.at(name);
    }
    else return nullptr;
}
