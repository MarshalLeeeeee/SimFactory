#pragma once

#ifndef __SIMCASE_H__
#define __SIMCASE_H__

#include "RenderObj.h"

#include <memory>
#include <unordered_map>
#include <chrono>

class SimEntity;

class SimCase {
public:
	SimCase();
	virtual ~SimCase();

	void update(ComPtr<ID3D11Device> dev);
	void render(ComPtr<ID3D11DeviceContext> devCon) const;

	virtual int getScreenWidth() const;
    virtual int getScreenHeight() const;
    virtual const WCHAR* getVsHLSL() const;
    virtual const WCHAR* getPsHLSL() const;

	bool addEntity(std::shared_ptr<SimEntity> pSimEntity, ComPtr<ID3D11Device> dev);
	void removeEntity(std::string uuid);
	bool hasEntity(std::string uuid) const;
	std::shared_ptr<SimEntity> getEntity(std::string uuid) const;

	bool addRenderObj(std::shared_ptr<RenderObj> pRenderObj, ComPtr<ID3D11Device> dev);
	void removeRenderObj(std::string uuid);
	bool hasRenderObj(std::string uuid) const;
	std::shared_ptr<RenderObj> getRenderObj(std::string uuid) const;

protected:
	virtual void doUpdate(ComPtr<ID3D11Device> dev, double simTime, double frameTime);
	void updateEntities(double simTime, double frameTime);

	std::unordered_map<std::string, std::shared_ptr<RenderObj>> renderObjs;
	std::unordered_map<std::string, std::shared_ptr<SimEntity>> entities;

	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> updateTime;
};

#include "SimEntity.h"

#endif
