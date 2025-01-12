/*
 * Ui widgets
 * Holding pSimCase
 */

#pragma once

#ifndef __UI_WIDGET_H__
#define __UI_WIDGET_H__

#include <string>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#include "SimCase.h"
#include "TypeUtil.h"

class UIWidget {
public:
    UIWidget(SimCase* pSimCase, std::string name);
    virtual ~UIWidget();
    /* render with all exclusive implementation */
    virtual void render() = 0;
    virtual Any getValue() const;
protected:
    SimCase* pSimCase;

public:
    std::string getName() const;
protected:
    std::string name;
};

class UISliderFloat : public UIWidget {
public:
    UISliderFloat(SimCase* pSimCase, std::string name, float v_min, float v_max);
    virtual ~UISliderFloat();

    void render();
    Any getValue() const;

protected:
    float val;
    float v_min;
    float v_max;
};

class UICheckbox : public UIWidget {
public:
    UICheckbox(SimCase* pSimCase, std::string name);
    virtual ~UICheckbox();

    void render();
    Any getValue() const;

protected:
    bool val;
};

class UIButton : public UIWidget {
public:
    UIButton(SimCase* pSimCase, std::string name);
    virtual ~UIButton();

    void render();
};

#endif
