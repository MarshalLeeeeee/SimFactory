#pragma once

#ifndef __FADINGTRIANGLECASE_H__
#define __FADINGTRIANGLECASE_H__

#include "SimCase.h"
#include "SimEntity.h"
#include "RenderObj.h"
#include "RenderGeometry.h"

#include <memory>

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

private:
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
