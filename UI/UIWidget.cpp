#include "UIWidget.h"

UIWidget::UIWidget(std::string uuid, std::string name) : 
    uuid(uuid), name(name) {}

UIWidget::~UIWidget() {}

std::string UIWidget::getUUID() const {
    return uuid;
}

UISliderFloat::UISliderFloat(std::string uuid, std::string name, float v_min, float v_max) :
    UIWidget(uuid, name), v_min(v_min), v_max(v_max), val(0.0) {}

UISliderFloat::~UISliderFloat() {}

void UISliderFloat::render() {
    ImGui::SliderFloat(name.c_str(), &val, v_min, v_max);
}

void UISliderFloat::getValue(Any& any) const {
    any = Any(val);
}

UICheckbox::UICheckbox(std::string uuid, std::string name) : 
    UIWidget(uuid, name), val(false) {}

UICheckbox::~UICheckbox() {}

void UICheckbox::render() {
    ImGui::Checkbox(name.c_str(), &val);
}

void UICheckbox::getValue(Any& any) const {
    any = Any(val);
}
