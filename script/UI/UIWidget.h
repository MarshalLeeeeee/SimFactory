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
    UIWidget(std::string name);
    virtual ~UIWidget();
    /* render with all exclusive implementation */
    virtual void render() = 0;
    virtual Any getValue() const;

public:
    std::string getName() const;
protected:
    std::string name;
};

class UISliderFloat : public UIWidget {
public:
    UISliderFloat(std::string name, float v_min, float v_max);
    ~UISliderFloat();

    void render();
    Any getValue() const;

private:
    float val;
    float v_min;
    float v_max;

};

class UICheckbox : public UIWidget {
public:
    UICheckbox(std::string name);
    ~UICheckbox();

    void render();
    Any getValue() const;

private:
    bool val;

};

#endif
