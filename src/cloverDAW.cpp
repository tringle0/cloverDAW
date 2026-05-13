#pragma once
#include "cloverDAW.h"
#include "app.h"

int main()
{
    App app;
    app.init();
    app.run();
    app.shutdown();

    return 0;
}