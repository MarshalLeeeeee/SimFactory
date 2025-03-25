/*
 * SimEntity
 * Holding model
 * Holding simulation property
 */

#pragma once

#ifndef __SIMENTITY_H__
#define __SIMENTITY_H__

#include "SimCase.h"
#include "Model.h"

#include <string>
#include <memory>

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

// class RenderEntityBase;

class SimEntity {
public:
    SimEntity();
    virtual ~SimEntity();

public:
    /* initialization of sim entity
     * uuid
     * init model
     * specific initialization
     */
    bool init(ComPtr<ID3D11Device> dev, SimCase* pSimCase, std::string uuid);
protected:
    /* intialization of model */
    virtual void initModel(ComPtr<ID3D11Device> dev, SimCase* pSimCase);
    /* intialization of specific demand */
    virtual bool initEntity();
    /* ptr of model */
    std::unique_ptr<Model> pModel;

public:
    /* update */
    void update(SimCase* pSimCase, double simTime, double frameTime);
    /* render */
    void render(ComPtr<ID3D11DeviceContext> devCon) const;
protected:
    /* update the logic property */
    virtual void updateProperty(SimCase* pSimCase, double simTime, double frameTime) = 0;
    /* update the render entity property */
    virtual void updateModel();

public:
    /* get uuid */
    std::string getUUID() const;
protected:
    /* uuid */
    std::string uuid;

};

// #include "RenderEntity.h"

#endif
