/*
 * A case generating various geometry shapes
 * Every shape has similarity transformation (rotation + scaling) locally
 * Shapes are generated randomly and moves directly (without collision)
 */

#pragma once

#ifndef __SIMPLESHAPECASE_H__
#define __SIMPLESHAPECASE_H__

#include "SimCase.h"
#include <vector>
#include <string>

class SimpleShapeCase : public SimCase {
public:
    SimpleShapeCase();
    virtual ~SimpleShapeCase();
	/* screen width defined by specific sim case */
    int getScreenWidth() const;
	/* screen height defined by specific sim case */
    int getScreenHeight() const;
private:
	/* implementation of the update of the logic properties */
    void doUpdate(ComPtr<ID3D11Device> dev, double simTime, double frameTime);

public:
    /* recycle expired entities to be removed in the next frame */
    void recycleEntity(std::string uuid);
private:
    std::vector<std::string> expiredEntities;
};

#endif
