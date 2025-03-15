#pragma once

#ifndef __MATHUTIL_H__
#define __MATHUTIL_H__

// No project header is allowed
#include <d3d11.h>
#include <DirectXMath.h>
// No project header is allowed

#define PI 3.1415926
#define PI_2 PI * 2.0

/* Get rotation matrix 4*4 in the XY surface */
DirectX::XMMATRIX getRotationMatrix(float angle);

/* Get scale matrix 4*4 for all axis */
DirectX::XMMATRIX getScalingMatrix(float sc);

/* Get scale matrix 4*4 for axis-X and axis-Y */
DirectX::XMMATRIX getScalingMatrix(float scX, float scY);

/* Get scale matrix 4*4 for axis-X, axis-Y, axis-Z */
DirectX::XMMATRIX getScalingMatrix(float scX, float scY, float scZ);

/* Get translation matrix 4*4 for axis-X and axis-Y */
DirectX::XMMATRIX getTranslationMatrix(float x, float y);

/* Get translation matrix 4*4 for axis-X, axis-Y, axis-Z */
DirectX::XMMATRIX getTranslationMatrix(float x, float y, float z);

/* Get transform matrix 4*4 for axis-X, axis-Y, axis-Z */
DirectX::XMMATRIX getTransformMatrix(float x, float y, float angle, float x2y);

/* Get transform matrix 4*4 for axis-X, axis-Y, axis-Z */
DirectX::XMMATRIX getTransformMatrix(float x, float y, float angle, float scX, float scY, float x2y);

#endif
