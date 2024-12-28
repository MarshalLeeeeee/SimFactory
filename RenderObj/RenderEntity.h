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

class RenderEntityBase {
public:
    RenderEntityBase(SimCase* pSimCase, std::string uuid);
    virtual ~RenderEntityBase();

    bool init(ComPtr<ID3D11Device> dev);
    virtual void updateField(uint32_t i, const std::string fieldName, const Any& anyValue) = 0;

protected:
    virtual bool initRenderObj(ComPtr<ID3D11Device> dev) = 0;

    SimCase* pSimCase;
    std::string uuid;
    std::unordered_set<std::string> renderObjs;
};

template<typename T>
class RenderEntity : public RenderEntityBase {
public:
    RenderEntity(SimCase* pSimCase, std::string uuid, 
        D3D_PRIMITIVE_TOPOLOGY* primitiveTopologyData, 
        DWORD** indicesData, 
        uint32_t* indicesLenData, uint32_t renderObjCnt, 
        T* vData, uint32_t vCnt) :
        RenderEntityBase(pSimCase, uuid),
        renderObjCnt(renderObjCnt), vertexCnt(vCnt) {
        primitiveTopologyArray = std::make_unique<D3D_PRIMITIVE_TOPOLOGY[]>(renderObjCnt);
        indicesArray = std::make_unique<DWORD*[]>(renderObjCnt);
        indicesLenArray = std::make_unique<uint32_t[]>(renderObjCnt);
        for (uint32_t i = 0; i < renderObjCnt; ++i) {
            primitiveTopologyArray[i] = primitiveTopologyData[i];
            indicesArray[i] = indicesData[i];
            indicesLenArray[i] = indicesLenData[i];
        }
        vertices = std::make_unique<T[]>(vertexCnt);
        for (uint32_t i = 0; i < vertexCnt; ++i) {
            vertices[i] = vData[i];
        }
    }

    ~RenderEntity() {}

    void updateField(uint32_t i, const std::string fieldName, const Any& anyValue) {
        if (i >= vertexCnt) return;
        vertices[i].updateField(fieldName, anyValue);
    }

private:
    bool initRenderObj(ComPtr<ID3D11Device> dev) {
        bool res = true;
        for (uint32_t i = 0; i < renderObjCnt; ++i) {
            std::string renderObjUUID = uuid + "-" + GenerateSubUUID();
            res &= pSimCase->addRenderObj(
                std::make_shared<RenderObj<T>>(
                    renderObjUUID,
                    L"vs.hlsl",
                    L"ps.hlsl",
                    primitiveTopologyArray[i],
                    vertices,
                    vertexCnt,
                    indicesArray[i],
                    indicesLenArray[i]
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

    std::shared_ptr<T[]> vertices;
    uint32_t vertexCnt;

    std::unique_ptr<D3D_PRIMITIVE_TOPOLOGY[]> primitiveTopologyArray;
    std::unique_ptr<DWORD*[]> indicesArray;
    std::unique_ptr<uint32_t[]>  indicesLenArray;
    uint32_t renderObjCnt;
};

#endif
