#include "MathUtil.h"

/* Get rotation matrix 4*4 in the XY surface */
DirectX::XMMATRIX getRotationMatrix(float angle) {
    return DirectX::XMMatrixRotationZ(angle);
}

/* Get scale matrix 4*4 for all axis */
DirectX::XMMATRIX getScalingMatrix(float sc) {
    return DirectX::XMMatrixScaling(sc, sc, sc);
}

/* Get scale matrix 4*4 for axis-X and axis-Y */
DirectX::XMMATRIX getScalingMatrix(float scX, float scY) {
    return DirectX::XMMatrixScaling(scX, scY, 1.f);
}

/* Get scale matrix 4*4 for axis-X, axis-Y, axis-Z */
DirectX::XMMATRIX getScalingMatrix(float scX, float scY, float scZ) {
    return DirectX::XMMatrixScaling(scX, scY, scZ);
}

/* Get translation matrix 4*4 for axis-X and axis-Y */
DirectX::XMMATRIX getTranslationMatrix(float x, float y) {
    return DirectX::XMMatrixTranslation(x, y, 0.f);
}

/* Get translation matrix 4*4 for axis-X, axis-Y, axis-Z */
DirectX::XMMATRIX getTranslationMatrix(float x, float y, float z) {
    return DirectX::XMMatrixTranslation(x, y, z);
}

/* Get transform matrix 4*4 for axis-X, axis-Y, axis-Z */
DirectX::XMMATRIX getTransformMatrix(float x, float y, float angle) {
    DirectX::XMMATRIX rotationMat = getRotationMatrix(angle);
    DirectX::XMMATRIX translationMat = getTranslationMatrix(x, y);
    return XMMatrixMultiply(rotationMat, translationMat);
}

/* Get transform matrix 4*4 for axis-X, axis-Y, axis-Z */
DirectX::XMMATRIX getTransformMatrix(float x, float y, float angle, float scX, float scY) {
    DirectX::XMMATRIX scalingMat = getScalingMatrix(scX, scY);
    DirectX::XMMATRIX rotationMat = getRotationMatrix(angle);
    DirectX::XMMATRIX translationMat = getTranslationMatrix(x, y);
    return XMMatrixMultiply(XMMatrixMultiply(scalingMat, rotationMat), translationMat);
}
