#pragma once

#ifndef __FADINGTRIANGLECASE_H__
#define __FADINGTRIANGLECASE_H__

#include "SimCase.h"
#include "SimEntity.h"
#include "RenderObj.h"
#include "RenderGeometry.h"

#include <memory>

#define PERIOD 2.0

class FadingTriangleCase : public SimCase {
public:
    FadingTriangleCase();
    virtual ~FadingTriangleCase();

    int getScreenWidth() const;
    int getScreenHeight() const;

private:
    void doUpdate(ComPtr<ID3D11Device> dev, double simTime, double frameTime);

    class FadingTriangle : public SimEntity {
    public:
        FadingTriangle(SimCase* pSimCase);
        virtual ~FadingTriangle();

    protected:
        bool initRenderObj(ComPtr<ID3D11Device> dev);
        bool initEntity();
        void updateProperty(double simTime, double frameTime);
        void updateRenderObj(std::shared_ptr<RenderObj>);

        float c;
    };
};

#endif
