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
        lpCmdLine,
        nCmdShow
    );
    if (!app.init()) return 0;
    return app.run();
}
