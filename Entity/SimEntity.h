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
    SimEntity(SimCase* pSimCase);
    virtual ~SimEntity();

    bool init(ComPtr<ID3D11Device> dev);
    void update(double simTime, double frameTime);
    std::string getUUID() const;

protected:
    bool initUUID();
    virtual bool initRenderObj(ComPtr<ID3D11Device> dev) = 0;
    virtual bool initEntity() = 0;
    virtual void updateRenderObj(std::shared_ptr<RenderObj> ro, double simTime, double frameTime) = 0;

    std::string uuid;
    SimCase* pSimCase;
};

#endif
