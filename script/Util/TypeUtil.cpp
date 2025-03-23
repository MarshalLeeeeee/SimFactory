#include "TypeUtil.h"
#include <locale>
#include <codecvt>
#include <cstdlib>

const D3D11_INPUT_ELEMENT_DESC VertexPosColor::inputLayout[2] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

VertexPosColor::VertexPosColor() {
    pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
    color = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
}

VertexPosColor::VertexPosColor(float x, float y, float z, float r, float g, float b, float a) {
    pos = DirectX::XMFLOAT3(x, y, z);
    color = DirectX::XMFLOAT4(r, g, b, a);
}

VertexPosColor::VertexPosColor(tinyxml2::XMLNode* node) {
    float x, y, z;
    float r, g, b, a;
    tinyxml2::XMLElement* p = node->FirstChildElement("Position");
    tinyxml2::XMLElement* c = node->FirstChildElement("Color");
    p->FirstChildElement("x")->QueryFloatText(&x);
    p->FirstChildElement("y")->QueryFloatText(&y);
    p->FirstChildElement("z")->QueryFloatText(&z);
    c->FirstChildElement("r")->QueryFloatText(&r);
    c->FirstChildElement("g")->QueryFloatText(&g);
    c->FirstChildElement("b")->QueryFloatText(&b);
    c->FirstChildElement("a")->QueryFloatText(&a);
    pos = DirectX::XMFLOAT3(x, y, z);
    color = DirectX::XMFLOAT4(r, g, b, a);
}

///////////////////////////////////////////////////

std::wstring getWStringFromString(std::string s) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring ws = converter.from_bytes(s.c_str());
    return ws;
}
