/*
 * SimEntity
 * Holding one render entity
 */

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
    SimEntity();
    virtual ~SimEntity();

public:
    /* initialization of sim entity
     * uuid
     * render entity
     * specific initialization
     */
    bool init(SimCase* pSimCase, ComPtr<ID3D11Device> dev);
protected:
    /* intialization of uuid */
    bool initUUID(SimCase* pSimCase);
    /* intialization of render entity */
    virtual bool initRenderEntity(SimCase* pSimCase, ComPtr<ID3D11Device> dev);
    /* intialization of specific demand */
    virtual bool initEntity();
    /* ptr of render entity */
    std::unique_ptr<RenderEntityBase> pRenderEntity;

public:
    /* update */
    void update(SimCase* pSimCase, double simTime, double frameTime);
protected:
    /* update the logic property */
    virtual void updateProperty(SimCase* pSimCase, double simTime, double frameTime) = 0;
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
