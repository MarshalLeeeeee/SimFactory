#pragma once

#ifndef __REDNERENTITY_H__
#define __RENDERENTITY_H__

#include "SimCase.h"
#include "SimUtil.h"
#include "TypeUtil.h"
#include "MathUtil.h"

#include <Windows.h>
#include <wrl/client.h>
#include <DirectXMath.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <string>
#include <unordered_set>

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

/*
 * Render entity manages render obj
 * Render entity has the common vertex layout and vertices data
 * Holding render obj uuids
 */
class RenderEntityBase {
public:
    RenderEntityBase(std::string uuid);
    virtual ~RenderEntityBase();
    /* update field of vertices data */
    virtual void updateField(uint32_t i, const std::string fieldName, const Any& anyValue) = 0;
protected:
    std::string uuid;

public:
    /* initialization */
    bool init(
        SimCase* pSimCase,
        ComPtr<ID3D11Device> dev,
        D3D_PRIMITIVE_TOPOLOGY* primitiveTopologyData,
        DWORD** indicesData,
        uint32_t* indicesLenData,
        uint32_t renderObjCnt
    );
protected:
    /* initialization of render objs */
    virtual bool initRenderObj(
        SimCase* pSimCase,
        ComPtr<ID3D11Device> dev,
        D3D_PRIMITIVE_TOPOLOGY* primitiveTopologyData,
        DWORD** indicesData,
        uint32_t* indicesLenData,
        uint32_t renderObjCnt
    ) = 0;
    /* uuid of render objs */
    std::unordered_set<std::string> renderObjs;

public:
    float getAngle() const { return angle; }
    void setAngle(float a) {
        angle = a;
        if (angle > PI_2) angle -= PI_2;
        else if (angle < 0.) angle += PI_2;
    }
    void updateTranformBuffer() {
        pTfBufferData->mat = getTransformMatrix(posX, posY, angle, scX, scY);
    }
protected:
    float posX; // NDC coordinate width
    float posY; // NDC coordinate height
    float posZ; // depth layer
    float scX; // local scaling in X
    float scY; // local scaling in Y
    float angle; // local rotation angle;
    std::shared_ptr<TransformBuffer> pTfBufferData; // transformBuffer data

protected:
    float opacity; // opacity
    bool vis; // visible, render or not
};

/*
 * Template cls inherited from RenderEntityBase
 * Holding configs of render objs
 * Holding vertices data
 */
template<typename T>
class RenderEntity : public RenderEntityBase {
public:
    RenderEntity(std::string uuid, T* vData, uint32_t vCnt) :
        RenderEntityBase(uuid), vertexCnt(vCnt) {
        vertices = std::make_unique<T[]>(vertexCnt);
        for (uint32_t i = 0; i < vertexCnt; ++i) {
            vertices[i] = vData[i];
        }
    }
    ~RenderEntity() {}
    /* update field of vertices data */
    void updateField(uint32_t i, const std::string fieldName, const Any& anyValue) {
        if (i >= vertexCnt) return;
        vertices[i].updateField(fieldName, anyValue);
    }
private:
    std::shared_ptr<T[]> vertices;
    uint32_t vertexCnt;

private:
    /* initialization of render objs */
    bool initRenderObj(
        SimCase* pSimCase,
        ComPtr<ID3D11Device> dev,
        D3D_PRIMITIVE_TOPOLOGY* primitiveTopologyData,
        DWORD** indicesData,
        uint32_t* indicesLenData,
        uint32_t renderObjCnt
    ) {
        bool res = true;
        for (uint32_t i = 0; i < renderObjCnt; ++i) {
            std::string renderObjUUID = uuid + "-" + generateSubUUID();
            res &= pSimCase->addRenderObj(
                std::make_shared<RenderObj<T>>(
                    renderObjUUID,
                    L"./res/shader/vs_VertexPosColor_raw.hlsl",
                    L"./res/shader/ps_VertexPosColor_raw.hlsl",
                    primitiveTopologyData[i],
                    vertices,
                    vertexCnt,
                    indicesData[i],
                    indicesLenData[i],
                    pTfBufferData
                ),
                dev
            );
            if (!res) {
                break;
            }
            else {
                renderObjs.insert(renderObjUUID);
            }
        }
        if (!res) {
            for (std::string renderObjUUID : renderObjs) pSimCase->removeRenderObj(renderObjUUID);
        }
        return res;
    }
};

#endif
