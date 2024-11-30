#pragma once

#ifndef __RENDERTRIANGLE_H__
#define __RENDERTRIANGLE_H__

#include "RenderObj.h"

class RenderTriangle : public RenderObj {
public:
	RenderTriangle(std::string uuid, double period);
	virtual ~RenderTriangle();

	bool init(ComPtr<ID3D11Device> dev);
	void update(double simTime, double frameTime);
	void render(ComPtr<ID3D11DeviceContext> devCon);

private:
	bool initDx(ComPtr<ID3D11Device> dev);
	void updateC(double simTime);

	float c;
	double period;
};

#endif
