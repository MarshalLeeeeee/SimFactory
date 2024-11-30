#include "Constant.h"
#include "SimApp.h"

// the entry point for any Windows program
int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
) {
    SimApp app(
        hInstance,
        nCmdShow,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        VS_HLSL,
        PS_HLSL
    );
    if (!app.init()) return 0;
    return app.run();
}
