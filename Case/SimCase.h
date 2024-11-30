#pragma once

#ifndef __SIMCASE_H__
#define __SIMCASE_H__

#include "RenderObj.h"

#include <memory>
#include <unordered_map>
#include <chrono>

class SimCase {
public:
	SimCase();
	virtual ~SimCase();

	void update(ComPtr<ID3D11Device> dev);
	void render(ComPtr<ID3D11DeviceContext> devCon);

protected:
	virtual void do_update(ComPtr<ID3D11Device> dev, double simTime, double frameTime) = 0;

	void addRenderObj(std::shared_ptr<RenderObj> pRenderObj, ComPtr<ID3D11Device> dev);
	void removeRenderObj(std::string uuid);

	std::unordered_map<std::string, std::shared_ptr<RenderObj>> renderObjs;

	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> updateTime;
};

#endif
