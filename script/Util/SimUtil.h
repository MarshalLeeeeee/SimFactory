#pragma once

#ifndef __SIMUTIL_H__
#define __SIMUTIL_H__

#include <random>
#include <sstream>
#include <vector>
#include <memory>
#include <Windows.h>

// Generate uuid
std::string GenerateUUID();

// Generate sub uuid
std::string GenerateSubUUID();

// Unparse lpCmdLine
void unparseLpCmdLine(LPSTR lpCmdLine, std::vector<std::string>& cmdArgs);

#endif
