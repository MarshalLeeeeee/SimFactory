#include "UIWidget.h"

//////////////////////////////////

UIWidget::UIWidget(SimCase* pSimCase, std::string name) : 
    pSimCase(pSimCase), name(name) {}

UIWidget::~UIWidget() {}

std::string UIWidget::getName() const {
    return name;
}

Any UIWidget::getValue() const {
    return Any(nullptr);
}

//////////////////////////////////

UISliderFloat::UISliderFloat(SimCase* pSimCase, std::string name, float v_min, float v_max, FunctionType func) :
    UIWidget(pSimCase, name), v_min(v_min), v_max(v_max), val(v_min), func(func) {}

UISliderFloat::~UISliderFloat() {}

void UISliderFloat::render() {
    ImGui::SliderFloat(name.c_str(), &val, v_min, v_max);
    func(val);
}

Any UISliderFloat::getValue() const {
    return Any(val);
}

//////////////////////////////////

UICheckbox::UICheckbox(SimCase* pSimCase, std::string name) : 
    UIWidget(pSimCase, name), val(false) {}

UICheckbox::~UICheckbox() {}

void UICheckbox::render() {
    ImGui::Checkbox(name.c_str(), &val);
}

Any UICheckbox::getValue() const {
    return Any(val);
}

//////////////////////////////////

UIButton::UIButton(SimCase* pSimCase, std::string name) : 
    UIWidget(pSimCase, name) {}

UIButton::~UIButton() {}

void UIButton::render() {
    if (ImGui::Button(name.c_str())) {
        ImGui::LogToClipboard();
        ImGui::LogText("Hello, world!");
        ImGui::LogFinish();
    }
}

//////////////////////////////////
