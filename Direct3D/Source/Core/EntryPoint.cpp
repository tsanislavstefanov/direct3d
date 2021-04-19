#include "stdafx.h"
#include "Application.h"
#include "Log.h"

extern Application* CreateApplication();

int main()
{
    Log::Init();

    Application* app = CreateApplication();
    app->Run();
    delete app;

    return 0;
}