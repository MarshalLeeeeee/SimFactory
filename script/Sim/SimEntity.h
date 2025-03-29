#pragma once

#ifndef __SIMENTITY_H__
#define __SIMENTITY_H__

#include "SimCase.h"
#include "Model.h"

#include <string>
#include <memory>

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

/*
 * SimEntity
 * Holding model
 * Holding simulation property
 */
class SimEntity {
public:
    SimEntity();
    SimEntity(float posX, float poxY, float angle, float scaleX, float scaleY);
    virtual ~SimEntity();

public:
    /* initialization of sim entity
     * specific initialization
     * init uuid
     * init model
     */
    bool init(ComPtr<ID3D11Device> dev, SimCase* pSimCase, std::string uuid);
protected:
    /* initialization of model */
    virtual void initModel(ComPtr<ID3D11Device> dev, SimCase* pSimCase);
    /* initialization of specific demand */
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
    /* update model */
    void updateModel(SimCase* pSimCase);
    /* update model (only when loaded) */
    virtual void doUpdateModel(SimCase* pSimCase);

public:
    /* get uuid */
    std::string getUUID() const;
protected:
    /* uuid */
    std::string uuid;

public:
    float getPosX() const;
    float getPosY() const;
    float getAngle() const;
    float getScaleX() const;
    float getScaleY() const;
    void setPosX(float posX);
    void setPosY(float posY);
    void setAngle(float angle);
    void setScaleX(float scaleX);
    void setScaleY(float scaleY);
protected:
    /* pos x in world coordinate */
    float posX;
    /* pos y in world coordinate */
    float posY;
    /* angle in world coordinate */
    float angle;
    /* scale x in world coordinate */
    float scaleX;
    /* scale y in world coordinate */
    float scaleY;
};

#endif
