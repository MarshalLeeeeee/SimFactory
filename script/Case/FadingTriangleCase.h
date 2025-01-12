#pragma once

#ifndef __FADINGTRIANGLECASE_H__
#define __FADINGTRIANGLECASE_H__

#include "SimCase.h"
#include "SimEntity.h"
#include "RenderObj.h"
#include "RenderObj.h"
#include "UIPanel.h"
#include "UIWidget.h"

#include <memory>
#include <functional>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "renderdoc/renderdoc_app.h"

class FadingTriangleCase : public SimCase {
public:
    FadingTriangleCase();
    virtual ~FadingTriangleCase();
    /* screen width defined by specific sim case */
    int getScreenWidth() const;
    /* screen height defined by specific sim case */
    int getScreenHeight() const;

public:
    void set_period(float p);
    /* changing period */
    float period;
    /* if use debug panel */
    bool show_debug_pnl;

private:
    /* if sim case needs ui */
    bool needUI() const;

private:
    /* pre stage of render */
    void preRender(ComPtr<ID3D11DeviceContext> devCon);
    /* post stage of render */
    void postRender(ComPtr<ID3D11DeviceContext> devCon);

private:
	/* implementation of the update of the logic properties */
    void doUpdate(ComPtr<ID3D11Device> dev, double simTime, double frameTime);

protected:
    /* initialization of case */
	bool initCase();
private:
    /* render doc api */
    RENDERDOC_API_1_6_0* rdoc_api;
    /* if should capture frame */
    bool should_capture_frame;
};

class FadingTriangle : public SimEntity {
public:
    FadingTriangle(FadingTriangleCase* pSimCase);
    virtual ~FadingTriangle();

private:
    /* intialization of render entity */
    bool initRenderEntity(ComPtr<ID3D11Device> dev);

private:
    /* update the logic property */
    void updateProperty(double simTime, double frameTime);
    /* update the render entity property */
    void updateRenderEntity();

private:
    /* color degree */
    float c;
    /* change direction */
    float d;
};

class FadingTriangleControlPanel : public UIPanel {
public:
    FadingTriangleControlPanel(SimCase* pSimcase, std::string name);
    ~FadingTriangleControlPanel();
};

class FadingTriangleDebugPanel : public UIPanel {
public:
    FadingTriangleDebugPanel(SimCase* pSimcase, std::string name);
    ~FadingTriangleDebugPanel();
};

#endif
