#pragma once

#ifndef __SIMENTITY_H__
#define __SIMENTITY_H__

#include "SimCase.h"

#include <string>
#include <memory>

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

class RenderEntityBase;

class SimEntity {
public:
    SimEntity(SimCase* pSimCase);
    virtual ~SimEntity();
protected:
    /* ptr of sim case */
    SimCase* pSimCase;

public:
    /* initialization of sim entity
     * uuid
     * render entity
     * specific initialization
     */
    bool init(ComPtr<ID3D11Device> dev);
protected:
    /* intialization of uuid */
    bool initUUID();
    /* intialization of render entity */
    virtual bool initRenderEntity(ComPtr<ID3D11Device> dev);
    /* intialization of specific demand */
    virtual bool initEntity();
    /* ptr of render entity */
    std::unique_ptr<RenderEntityBase> pRenderEntity;

public:
    /* update */
    void update(double simTime, double frameTime);
protected:
    /* update the logic property */
    virtual void updateProperty(double simTime, double frameTime) = 0;
    /* update the render entity property */
    virtual void updateRenderEntity();

public:
    /* get uuid */
    std::string getUUID() const;
protected:
    /* uuid */
    std::string uuid;

};

#include "RenderEntity.h"

#endif
