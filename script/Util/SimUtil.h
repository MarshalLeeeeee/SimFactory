#pragma once

#ifndef __SIMUTIL_H__
#define __SIMUTIL_H__

// No project header is allowed
#include <random>
#include <sstream>
#include <vector>
#include <memory>
#include <Windows.h>
// No project header is allowed

// Generate uuid
std::string GenerateUUID();

// Generate sub uuid
std::string GenerateSubUUID();

// Unparse lpCmdLine
void unparseLpCmdLine(LPSTR lpCmdLine, std::vector<std::string>& cmdArgs);

#endif
