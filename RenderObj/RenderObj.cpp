#include "RenderObj.h"

RenderObj::RenderObj(std::string uuid) :
	vBuffer(nullptr), uuid(uuid), renderReady(false) {}

RenderObj::~RenderObj() {}

std::string RenderObj::getUUID() const {
	return uuid;
}
