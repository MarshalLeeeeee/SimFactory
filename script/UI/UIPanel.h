/*
 * UI panel
 */

#pragma once

#ifndef __UIPANEL_H__
#define __UIPANEL_H__

#include <memory>
#include <string>
#include <unordered_map>

#include "UIWidget.h"

class UIPanel {
public:
    UIPanel(std::string name);
    virtual ~UIPanel();
    /* render of all including widgets */
    void render() const;

public:
    std::string getName() const;
private:
    std::string name;

public:
    bool addUIWidget(std::shared_ptr<UIWidget> pUIWidget);
    bool removeUIWidget(std::string name);
    bool hasUIWidget(std::string name) const;
    std::shared_ptr<UIWidget> getUIWidget(std::string name) const;
private:
    /* all widgets with exclusive names as key */
    std::unordered_map<std::string, std::shared_ptr<UIWidget>> uiWidgets;

};

#endif
