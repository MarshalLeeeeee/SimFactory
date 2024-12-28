#pragma once

#ifndef __FADINGTRIANGLECASE_H__
#define __FADINGTRIANGLECASE_H__

#include "SimCase.h"
#include "SimEntity.h"
#include "RenderObj.h"
#include "RenderGeometry.h"

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
    bool initRenderObj(ComPtr<ID3D11Device> dev);
    bool initEntity();
    void updateProperty(double simTime, double frameTime);
    void updateRenderObj(std::shared_ptr<RenderObj>);

private: // sim logic property
    float c;
    float d;

private: // render property
    std::shared_ptr<VertexPosColor[]> vertices;
    uint32_t vertexCnt;

    std::shared_ptr<DWORD[]> indices;
    uint32_t indexCnt;
};

class FadingTriangleCase : public SimCase {
public:
    FadingTriangleCase();
    virtual ~FadingTriangleCase();

    int getScreenWidth() const;
    int getScreenHeight() const;

    float getPeriod() const;

private:
    bool needUI() const;
    void doUpdate(ComPtr<ID3D11Device> dev, double simTime, double frameTime);

    std::string sliderUUID;
    std::string checkboxUUID;

    float period;
};

#endif
