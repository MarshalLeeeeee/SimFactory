#pragma once

#ifndef __UI_WIDGET_H__
#define __UI_WIDGET_H__

#include <string>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "TypeUtil.h"

class UIWidget {
public:
    UIWidget(std::string uuid, std::string name);
    virtual ~UIWidget();

    virtual void render() = 0;

    std::string getUUID() const;
    virtual void getValue(Any& any) const = 0;

protected:
    std::string uuid;
    std::string name;
};

class UISliderFloat : public UIWidget {
public:
    UISliderFloat(std::string uuid, std::string name, float v_min, float v_max);
    ~UISliderFloat();

    void render();
    void getValue(Any& any) const;

private:
    float val;
    float v_min;
    float v_max;

};

class UICheckbox : public UIWidget {
public:
    UICheckbox(std::string uuid, std::string name);
    ~UICheckbox();

    void render();
    void getValue(Any& any) const;

private:
    bool val;

};

#endif
