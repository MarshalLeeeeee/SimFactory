#include "UIWidget.h"

UIWidget::UIWidget(std::string uuid, std::string name) : 
    uuid(uuid), name(name) {}

UIWidget::~UIWidget() {}

std::string UIWidget::getUUID() const {
    return uuid;
}

UISliderFloat::UISliderFloat(std::string uuid, std::string name, float v_min, float v_max) :
    UIWidget(uuid, name), v_min(v_min), v_max(v_max), val(v_min) {}

UISliderFloat::~UISliderFloat() {}

void UISliderFloat::render() {
    ImGui::SliderFloat(name.c_str(), &val, v_min, v_max);
}

Any UISliderFloat::getValue() const {
    return Any(val);
}

UICheckbox::UICheckbox(std::string uuid, std::string name) : 
    UIWidget(uuid, name), val(false) {}

UICheckbox::~UICheckbox() {}

void UICheckbox::render() {
    ImGui::Checkbox(name.c_str(), &val);
}

Any UICheckbox::getValue() const {
    return Any(val);
}
