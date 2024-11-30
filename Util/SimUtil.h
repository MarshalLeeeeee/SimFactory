#pragma once

#ifndef __SIMUTIL_H__
#define __SIMUTIL_H__

#include <random>
#include <sstream>
#include <vector>
#include <memory>
#include <Windows.h>

#include "SimCase.h"

// Generate uuid for render objects
std::string GenerateRenderObjUUID();

// Unparse lpCmdLine
void unparseLpCmdLine(LPSTR lpCmdLine, std::vector<std::string>& cmdArgs);

// SimCase factory
std::shared_ptr<SimCase> createSimCase(const std::vector<std::string>& cmdArgs);

#endif
