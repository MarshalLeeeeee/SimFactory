#include "UIWidget.h"

//////////////////////////////////

UIWidget::UIWidget(std::string name) : 
    name(name) {}

UIWidget::~UIWidget() {}

std::string UIWidget::getName() const {
    return name;
}

//////////////////////////////////

UISliderFloat::UISliderFloat(std::string name, float v_min, float v_max, FunctionType func) :
    UIWidget(name), v_min(v_min), v_max(v_max), val(v_min), func(func) {}

UISliderFloat::~UISliderFloat() {}

void UISliderFloat::render() {
    ImGui::SliderFloat(name.c_str(), &val, v_min, v_max);
    func(val);
}

//////////////////////////////////

UICheckbox::UICheckbox(std::string name, FunctionType func) : 
    UIWidget(name), val(false), func(func) {}

UICheckbox::~UICheckbox() {}

void UICheckbox::render() {
    ImGui::Checkbox(name.c_str(), &val);
    func(val);
}

//////////////////////////////////

UIButton::UIButton(std::string name, FunctionType func) : 
    UIWidget(name), func(func) {}

UIButton::~UIButton() {}

void UIButton::render() {
    if (ImGui::Button(name.c_str())) {
        ImGui::LogToClipboard();
        ImGui::LogText("Hello, world!");
        ImGui::LogFinish();
    }
}

//////////////////////////////////
