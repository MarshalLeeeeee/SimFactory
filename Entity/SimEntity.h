#pragma once

#ifndef __SIMENTITY_H__
#define __SIMENTITY_H__

#include "RenderObj.h"
#include "SimCase.h"

#include <string>
#include <memory>

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

class SimEntity {
public:
    SimEntity(ComPtr<ID3D11Device> dev, SimCase* pSimCase);
    virtual ~SimEntity();

    void update(double simTime, double frameTime);
    std::string getUUID() const;

protected:
    void initUUID();
    virtual void initRenderObj(ComPtr<ID3D11Device> dev) = 0;
    virtual void updateRenderObj(std::shared_ptr<RenderObj> ro, double simTime, double frameTime) = 0;

    std::string uuid;
    SimCase* pSimCase;
};

#endif
