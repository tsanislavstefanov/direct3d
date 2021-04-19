#pragma once

#include "Core/Application.h"
#include "Core/Window.h"

class Sandbox : public Application
{
public:
    Sandbox(const WindowProps& props);
protected:
    void OnUpdate() override;
private:
    void OnGUI();
};