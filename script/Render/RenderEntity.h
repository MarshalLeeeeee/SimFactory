#pragma once

#ifndef __REDNERENTITY_H__
#define __RENDERENTITY_H__

#include "SimCase.h"
#include "SimUtil.h"
#include "TypeUtil.h"

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
    RenderEntityBase(std::string uuid, float x2y_);
    virtual ~RenderEntityBase();
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
    float getAngle() const;
    void setAngle(float a);
    void updateTranformBuffer();
protected:
    float x2y; // screen resolution x : y
    float posX; // NDC coordinate width
    float posY; // NDC coordinate height
    float posZ; // depth layer
    float scX; // local scaling in X
    float scY; // local scaling in Y
    float angle; // local rotation angle;
    std::shared_ptr<TransformBuffer> pTfBufferData; // transformBuffer data
    
public:
    float getOpacity() const;
    void setOpacity(float o);
    float getIntensity() const;
    void setIntensity(float i);
    void updatePixelBuffer();
protected:
    float opacity; // opacity
    float intensity; // intensity
    std::shared_ptr<PixelBuffer> pPxBufferData; // pixelBuffer data
};

/*
 * Template cls inherited from RenderEntityBase
 * Holding configs of render objs
 * Holding vertices data
 */
template<typename T>
class RenderEntity : public RenderEntityBase {
public:
    RenderEntity(std::string uuid, float x2y, T* vData, uint32_t vCnt) :
        RenderEntityBase(uuid, x2y), vertexCnt(vCnt) {
        vertices = std::make_unique<T[]>(vertexCnt);
        for (uint32_t i = 0; i < vertexCnt; ++i) {
            vertices[i] = vData[i];
        }
    }
    virtual ~RenderEntity() {}
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
                    L"./res/shader/vs_poscolor_raw.hlsl",
                    L"./res/shader/ps_poscolor_raw.hlsl",
                    primitiveTopologyData[i],
                    vertices,
                    vertexCnt,
                    indicesData[i],
                    indicesLenData[i],
                    pTfBufferData,
                    pPxBufferData
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
