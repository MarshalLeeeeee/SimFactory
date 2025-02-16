#include "SimUtil.h"

////////////////////////
// Generate uuid for render objects
static std::random_device              rd;
static std::mt19937                    gen(rd());
static std::uniform_int_distribution<> dis(0, 15);
static std::uniform_int_distribution<> dis2(8, 11);

// Generate uuid
std::string generateUUID() {
    std::stringstream ss;
    int i;
    ss << std::hex;
    for (i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen);
    for (i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 4; i++) {
        ss << dis(gen);
    };
    return ss.str();
}

// Generate sub uuid
std::string generateSubUUID() {
    std::stringstream ss;
    int i;
    ss << std::hex;
    for (i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    return ss.str();
}

// Generate uniform random within [0,1]
double uniformRandom() {
    return distribution(generator);
}


////////////////////////
// Unparse lpCmdLine
void unparseLpCmdLine(LPSTR lpCmdLine, std::vector<std::string>& cmdArgs) {
    cmdArgs.clear();
    bool inQuotes = false;
    std::string arg;
    for (char* c = lpCmdLine; *c; ++c) {
        if (*c == '\"') {
            if (inQuotes) {
                if (!arg.empty()) {
                    cmdArgs.push_back(arg);
                    arg.clear();
                }
                inQuotes = false;
            }
            else {
                inQuotes = true;
            }
        }
        else {
            if (*c == ' ' && !inQuotes) {
                if (!arg.empty()) {
                    cmdArgs.push_back(arg);
                    arg.clear();
                }
            }
            else {
                arg += *c;
            }
        }
    }
    if (!arg.empty()) {
        cmdArgs.push_back(arg);
    }
}
