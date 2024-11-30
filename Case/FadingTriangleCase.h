#pragma once

#ifndef __FADINGTRIANGLECASE_H__
#define __FADINGTRIANGLECASE_H__

#include "SimCase.h"

#define PERIOD 10.0

class FadingTriangleCase : public SimCase {
public:
    FadingTriangleCase();
    virtual ~FadingTriangleCase();

private:
    void do_update(ComPtr<ID3D11Device> dev, double simTime, double frameTime);
};

#endif
