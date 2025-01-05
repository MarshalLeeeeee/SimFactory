#include "UIWidget.h"

//////////////////////////////////

UIWidget::UIWidget(std::string name) : 
    name(name) {}

UIWidget::~UIWidget() {}

std::string UIWidget::getName() const {
    return name;
}

Any UIWidget::getValue() const {
    return Any(nullptr);
}

//////////////////////////////////

UISliderFloat::UISliderFloat(std::string name, float v_min, float v_max) :
    UIWidget(name), v_min(v_min), v_max(v_max), val(v_min) {}

UISliderFloat::~UISliderFloat() {}

void UISliderFloat::render() {
    ImGui::SliderFloat(name.c_str(), &val, v_min, v_max);
}

Any UISliderFloat::getValue() const {
    return Any(val);
}

//////////////////////////////////

UICheckbox::UICheckbox(std::string name) : 
    UIWidget(name), val(false) {}

UICheckbox::~UICheckbox() {}

void UICheckbox::render() {
    ImGui::Checkbox(name.c_str(), &val);
}

Any UICheckbox::getValue() const {
    return Any(val);
}

//////////////////////////////////
