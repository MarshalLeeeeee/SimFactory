/*
 * Ui widgets
 */

#pragma once

#ifndef __UI_WIDGET_H__
#define __UI_WIDGET_H__

#include <string>
#include <functional>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

class UIWidget {
public:
    UIWidget(std::string name);
    virtual ~UIWidget();
    /* render with all exclusive implementation */
    virtual void render() = 0;

public:
    std::string getName() const;
protected:
    std::string name;
};

class UISliderFloat : public UIWidget {
public:
    using FunctionType = std::function<void(float)>;
    UISliderFloat(std::string name, float v_min, float v_max, FunctionType func);
    virtual ~UISliderFloat();
private:
    FunctionType func;

public:
    void render();
protected:
    float val;
    float v_min;
    float v_max;
};

class UICheckbox : public UIWidget {
public:
    using FunctionType = std::function<void(bool)>;
    UICheckbox(std::string name, FunctionType func);
    virtual ~UICheckbox();
private:
    FunctionType func;

public:
    void render();
protected:
    bool val;
};

class UIButton : public UIWidget {
public:
    using FunctionType = std::function<void(void)>;
    UIButton(std::string name, FunctionType func);
    virtual ~UIButton();
private:
    FunctionType func;

public:
    void render();
};

#endif
