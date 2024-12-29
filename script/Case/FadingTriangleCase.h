#pragma once

#ifndef __FADINGTRIANGLECASE_H__
#define __FADINGTRIANGLECASE_H__

#include "SimCase.h"
#include "SimEntity.h"
#include "RenderObj.h"
#include "RenderObj.h"

#include <memory>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

class FadingTriangle : public SimEntity {
public:
    FadingTriangle(SimCase* pSimCase);
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

class FadingTriangleCase : public SimCase {
public:
    FadingTriangleCase();
    virtual ~FadingTriangleCase();
    /* screen width defined by specific sim case */
    int getScreenWidth() const;
    /* screen height defined by specific sim case */
    int getScreenHeight() const;

private:
    /* if sim case needs ui */
    bool needUI() const;
    /* uuid of slider */
    std::string sliderUUID;
    /* uuid of checkbox */
    std::string checkboxUUID;

private:
	/* implementation of the update of the logic properties */
    void doUpdate(ComPtr<ID3D11Device> dev, double simTime, double frameTime);

public:
    /* get changing period */
    float getPeriod() const;
private:
    /* changing period */
    float period;
};

#endif
