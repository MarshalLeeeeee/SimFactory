#pragma once

#ifndef __FADINGTRIANGLECASE_H__
#define __FADINGTRIANGLECASE_H__

#include "SimCase.h"
#include "SimEntity.h"
#include "RenderObj.h"
#include "UIPanel.h"
#include "UIWidget.h"

#include <memory>
#include <functional>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifndef NDEBUG
#include "renderdoc/renderdoc_app.h"
#endif

class FadingTriangleCase : public SimCase {
public:
    FadingTriangleCase();
    virtual ~FadingTriangleCase();
    /* screen width defined by specific sim case */
    int getScreenWidth() const;
    /* screen height defined by specific sim case */
    int getScreenHeight() const;

public:
    void setPeriod(float p);
    float getPeriod() const;
private:
    /* changing period */
    float period;

#ifndef NDEBUG
public:
    void setShowDebugPnl(bool b);
private:
    /* if use debug panel */
    bool showDebugPnl;
#endif

private:
    /* if sim case needs ui */
    bool needUI() const;

private:
    /* pre stage of render */
    void preRender(ComPtr<ID3D11Device> dev, HWND hWindow);
    /* post stage of render */
    void postRender(ComPtr<ID3D11Device> dev, HWND hWindow);

private:
	/* implementation of the update of the logic properties */
    void doUpdate(ComPtr<ID3D11Device> dev, double simTime, double frameTime);

public:
    /* pre initialization of the sim case */
    bool preInit();
private:
    /* init render doc */
    bool initRenderDoc();

#ifndef NDEBUG
public:
    /* enable capture frame once */
    void enableCaptureFrame();
private:
    /* render doc api */
    RENDERDOC_API_1_6_0* renderDocApi;
    /* if should capture frame */
    bool captureFrameSwitch;
    /* if capture frame starts */
    bool captureFraming;
#endif
};

class FadingTriangle : public SimEntity {
public:
    FadingTriangle();
    virtual ~FadingTriangle();

private:
    /* intialization of render entity */
    bool initRenderEntity(SimCase* pSimCase, ComPtr<ID3D11Device> dev);

private:
    /* update the logic property */
    void updateProperty(SimCase* pSimCase, double simTime, double frameTime);
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
    FadingTriangleControlPanel(std::string name, FadingTriangleCase* pSimCase);
    ~FadingTriangleControlPanel();
};

class FadingTriangleDebugPanel : public UIPanel {
public:
    FadingTriangleDebugPanel(std::string name, FadingTriangleCase* pSimCase);
    ~FadingTriangleDebugPanel();
};

#endif
