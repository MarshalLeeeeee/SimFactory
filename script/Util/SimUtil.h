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

/* random generator */
static std::mt19937 generator(std::random_device{}());

/* uniform distribution within [0., 1.] */
static std::uniform_real_distribution<double> distribution(0.0, 1.0);

/* generate uuid */
std::string generateUUID();

/* generate sub uuid */
std::string generateSubUUID();

/* Generate uniform random within [0,1] */
double uniformRandom();

/* Unparse lpCmdLine */
void unparseLpCmdLine(LPSTR lpCmdLine, std::vector<std::string>& cmdArgs);

#endif
