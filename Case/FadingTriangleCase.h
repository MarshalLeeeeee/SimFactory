#pragma once

#ifndef __FADINGTRIANGLECASE_H__
#define __FADINGTRIANGLECASE_H__

#include "SimCase.h"
#include "SimEntity.h"
#include "RenderTriangle.h"

#include <memory>

#define PERIOD 10.0

class FadingTriangleCase : public SimCase {
public:
    FadingTriangleCase();
    virtual ~FadingTriangleCase();

private:
    void doUpdate(ComPtr<ID3D11Device> dev, double simTime, double frameTime);

    class FadingTriangle : public SimEntity {
    public:
        FadingTriangle(ComPtr<ID3D11Device> dev, SimCase* pSimCase);
        virtual ~FadingTriangle();

    protected:
        void initRenderObj(ComPtr<ID3D11Device> dev);
        void updateRenderObj(std::shared_ptr<RenderObj>, double simTime, double frameTime);
    };
    std::shared_ptr<FadingTriangle> fadingTriangle;
};

#endif
