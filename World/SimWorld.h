#pragma once

#ifndef __SIMWORLD_H__
#define __SIMWORLD_H__

#include "RenderObj.h"

#include <memory>
#include <unordered_map>
#include <chrono>

class SimWorld {
public:
	SimWorld();
	virtual ~SimWorld();

	void update(ComPtr<ID3D11Device> dev);
	void render(ComPtr<ID3D11DeviceContext> devCon);

private:
	void addRenderObj(std::shared_ptr<RenderObj> pRenderObj, ComPtr<ID3D11Device> dev);
	void removeRenderObj(std::string uuid);

	std::unordered_map<std::string, std::shared_ptr<RenderObj>> renderObjs;

	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> updateTime;
};

#endif
