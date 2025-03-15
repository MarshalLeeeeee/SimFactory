#pragma once

#ifndef __TYPE_UTIL_H__
#define __TYPE_UTIL_H__

// No project header is allowed
#include <string>
#include <memory>
#include <d3d11.h>
#include <DirectXMath.h>
// No project header is allowed

/* class holding arbitary type */
class Any {
public:
    Any() : content(nullptr) {}

    template<typename T>
    Any(T value) : content(new Holder<T>(value)) {}

    template<typename T>
    T get() const {
        if (content->type() != typeid(T)) {
            throw std::bad_cast();
        }
        return static_cast<Holder<T>*>(content.get())->held;
    }

private:
    struct Base {
        virtual ~Base() = default;
        virtual const std::type_info& type() const = 0;
        virtual Base* clone() const = 0;
    };

    template<typename T>
    struct Holder : public Base {
        Holder(T value) : held(value) {}
        const std::type_info& type() const { return typeid(T); }
        Base* clone() const { return new Holder(held); }
        T held;
    };

    std::unique_ptr<Base> content;
};

/* Layout type : VertexPosColor */
struct VertexPosColor {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
};

/* Transform Buffer */
struct TransformBuffer {
    DirectX::XMMATRIX mat;
};

/* Pixel Buffer */
struct PixelBuffer {
    DirectX::XMFLOAT4 data;
};

#endif
