#pragma once

#ifndef __SIMCASE_H__
#define __SIMCASE_H__

#include "RenderObj.h"
#include "UI.h"

#include <memory>
#include <unordered_map>
#include <chrono>

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

class SimEntity;

class SimCase {
public:
	SimCase();
	virtual ~SimCase();

	bool init(HWND hWindow, ComPtr<ID3D11Device> dev, ComPtr<ID3D11DeviceContext> devCon);
	void update(ComPtr<ID3D11Device> dev);
	void preRender(ComPtr<ID3D11DeviceContext> devCon) const;
	void render(ComPtr<ID3D11DeviceContext> devCon) const;

	virtual int getScreenWidth() const;
    virtual int getScreenHeight() const;

	bool addEntity(std::shared_ptr<SimEntity> pSimEntity, ComPtr<ID3D11Device> dev);
	bool removeEntity(std::string uuid);
	bool hasEntity(std::string uuid) const;
	std::shared_ptr<SimEntity> getEntity(std::string uuid) const;

	bool addRenderObj(std::shared_ptr<RenderObjBase> pRenderObj, ComPtr<ID3D11Device> dev);
	bool removeRenderObj(std::string uuid);
	bool hasRenderObj(std::string uuid) const;
	std::shared_ptr<RenderObjBase> getRenderObj(std::string uuid) const;

	LRESULT CALLBACK preProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	virtual bool needUI() const;
	bool initUI(HWND hWindow, ComPtr<ID3D11Device> dev, ComPtr<ID3D11DeviceContext> devCon);

	virtual void doUpdate(ComPtr<ID3D11Device> dev, double simTime, double frameTime);
	void updateEntities(double simTime, double frameTime);

	std::unordered_map<std::string, std::shared_ptr<RenderObjBase>> renderObjs;
	std::unordered_map<std::string, std::shared_ptr<SimEntity>> entities;

	std::shared_ptr<UI> pUI;

	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> updateTime;
};

#include "SimEntity.h"

#endif
