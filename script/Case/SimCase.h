/*
 * SimCase frame
 * Holding UI
 * Holding sim entities
 * Holding render objs
 */
#pragma once

#ifndef __SIMCASE_H__
#define __SIMCASE_H__

#include "UI.h"

#include <memory>
#include <unordered_map>
#include <chrono>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

class Entity;

class SimCase {
public:
	SimCase();
	virtual ~SimCase();
	/* screen width defined by specific sim case */
	virtual int getScreenWidth() const;
	/* screen height defined by specific sim case */
	virtual int getScreenHeight() const;
	/* screen resolution x : y */
	float width2height;
protected:
	/* start time of the simulation */
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	/* time of the last update */
	std::chrono::time_point<std::chrono::high_resolution_clock> updateTime;

public:
	/*
	* pre initialization of the sim case
	* before window and dx
	*/
	virtual bool preInit();
	/* 
	* initialization of the sim case 
	* including ui
	*/
	bool init(HWND hWindow, ComPtr<ID3D11Device> dev, ComPtr<ID3D11DeviceContext> devCon);
	/* event handler of sim case */
	LRESULT CALLBACK simProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
protected:
	/* initialization of ui */
	bool initUI(HWND hWindow, ComPtr<ID3D11Device> dev, ComPtr<ID3D11DeviceContext> devCon);
	/* if sim case needs ui */
	virtual bool needUI() const;
	/* ptr of UI */
	std::unique_ptr<UI> pUI;

public:
	/* update of the logic properties */
	void update(ComPtr<ID3D11Device> dev);
	/* pre stage of render */
	virtual void preRender(ComPtr<ID3D11Device> dev, HWND hWindow);
	/* stage of render 
	* draw meshes and ui
	*/
	void render(ComPtr<ID3D11DeviceContext> devCon) const;
	/* post stage of render */
	virtual void postRender(ComPtr<ID3D11Device> dev, HWND hWindow);
protected:
	/* implementation of the update of the logic properties */
	virtual void doUpdate(ComPtr<ID3D11Device> dev, double simTime, double frameTime);

public:
	/* add sim entity */
	bool addEntity(ComPtr<ID3D11Device> dev, std::shared_ptr<Entity> pSimEntity);
	/* remove sim entity */
	bool removeEntity(std::string uuid);
	/* if has sim entity */
	bool hasEntity(std::string uuid) const;
	/* get sim entity, nullptr if not exist */
	std::shared_ptr<Entity> getEntity(std::string uuid) const;
protected:
	/* update logic properties of sim entities */
	void updateEntities(double simTime, double frameTime);
	/* uuid -> sim entity */
	std::unordered_map<std::string, std::shared_ptr<Entity>> entities;
};

#include "Entity.h"

#endif
