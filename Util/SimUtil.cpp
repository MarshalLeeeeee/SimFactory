#include "SimUtil.h"

#include "SimCase.h"
#include "FadingTriangleCase.h"

////////////////////////
// Generate uuid for render objects
static std::random_device              rd;
static std::mt19937                    gen(rd());
static std::uniform_int_distribution<> dis(0, 15);
static std::uniform_int_distribution<> dis2(8, 11);

std::string GenerateRenderObjUUID() {
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

////////////////////////
// SimCase factory
std::shared_ptr<SimCase> createSimCase(const std::vector<std::string>& cmdArgs) {
    if (cmdArgs.empty()) return std::make_shared<SimCase>();
    else {
        std::string caseName = cmdArgs[0];
        if (caseName == "FadingTriangleCase") return std::make_shared<FadingTriangleCase>();
        else return std::make_shared<SimCase>();
    }
}
