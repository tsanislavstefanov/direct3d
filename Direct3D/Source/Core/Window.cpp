#include "stdafx.h"
#include "Window.h"

////////////////////////////////
// WINDOW //////////////////////
////////////////////////////////

Window::Window(const WindowProps& props)
{
    // Set data.
    m_Data.Width = props.Width;
    m_Data.Height = props.Height;
    m_Data.VSync = props.VSync;
    m_Data.Dispatcher = &Dispatcher;

    // Get application module.
    m_Module = GetModuleHandleW(0);

    // Create and register class.
    const WCHAR* class_name = L"WindowClass";
    WNDCLASSEXW wcexw;
    memset(&wcexw, 0, sizeof(WNDCLASSEXW));
    wcexw.cbSize = sizeof(WNDCLASSEXW);
    wcexw.style = CS_OWNDC;
    wcexw.lpfnWndProc = [](HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) -> LRESULT {
        // Get user data.
        const LONG usrptr = GetWindowLongPtrW(hwnd, GWLP_USERDATA);
        auto& data = *reinterpret_cast<WindowData*>(usrptr);

        // Dispatch event(s).
        switch (umsg)
        {
            case WM_MOUSEMOVE:
            {
                const auto x = (FLOAT)GET_X_LPARAM(lparam), y = (FLOAT)GET_Y_LPARAM(lparam);
                MouseMovedEvent e({ x, y });
                data.Dispatcher->Publish(e);
                break;
            }
            case WM_MOUSEWHEEL:
            {
                const auto y = (FLOAT)HIWORD(wparam) / (FLOAT)WHEEL_DELTA;
                MouseWheelMovedEvent e({ 0.0f, y });
                data.Dispatcher->Publish(e);
                break;
            }
            case WM_LBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_RBUTTONDOWN:
            {
                const auto button_code = (UINT)wparam;
                MouseButtonDownEvent e(button_code);
                data.Dispatcher->Publish(e);
                break;
            }
            case WM_LBUTTONUP:
            case WM_MBUTTONUP:
            case WM_RBUTTONUP:
            {
                const auto button_code = (UINT)wparam;
                MouseButtonUpEvent e(button_code);
                data.Dispatcher->Publish(e);
                break;
            }
            case WM_KEYDOWN:
            case WM_KEYUP:
            {
                const auto key_code = (UINT)wparam;
                if (HIWORD(lparam) & KF_UP)
                {
                    KeyUpEvent e(key_code);
                    data.Dispatcher->Publish(e);
                }
                else if (HIWORD(lparam) & KF_REPEAT)
                {
                    KeyDownEvent e(key_code, 1);
                    data.Dispatcher->Publish(e);
                }
                else
                {
                    KeyDownEvent e(key_code, 0);
                    data.Dispatcher->Publish(e);
                }
                break;
            }
            case WM_SIZE:
            {
                const auto width = (UINT)LOWORD(lparam);
                const auto height = (UINT)HIWORD(lparam);
                if (data.Width == width && data.Height == height)
                    break;

                data.Width = width;
                data.Height = height;
                data.GraphicsContext->ResizeBuffers();
                WindowResizedEvent e(width, height);
                data.Dispatcher->Publish(e);
                break;
            }
            case WM_DESTROY:
            {
                WindowCloseEvent e = {};
                data.Dispatcher->Publish(e);
                PostQuitMessage(0);
                break;
            }
        }

        // Use default procedure.
        return DefWindowProcW(hwnd, umsg, wparam, lparam);
    };
    wcexw.cbClsExtra = 0;
    wcexw.cbWndExtra = 0;
    wcexw.hInstance = m_Module;
    wcexw.hIcon = nullptr;
    wcexw.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcexw.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wcexw.lpszMenuName = L"";
    wcexw.lpszClassName = class_name;
    wcexw.hIconSm = nullptr;
    RegisterClassExW(&wcexw);

    // Create and adjust rect.
    RECT rect;
    memset(&rect, 0, sizeof(RECT));
    rect.left = 0;
    rect.top = 0;
    rect.right = (LONG)props.Width;
    rect.bottom = (LONG)props.Height;
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    // Create handle.
    m_Handle = CreateWindowExW(
        0,                                              // Optional styles.
        class_name,                                     // Class name.
        &props.Title[0],                                // Title.
        WS_OVERLAPPEDWINDOW,                            // Style.
        CW_USEDEFAULT, CW_USEDEFAULT,                   // Position.
        rect.right - rect.left, rect.bottom - rect.top, // Size.
        nullptr,                                        // Parent.
        nullptr,                                        // Menu.
        m_Module,                                       // Module.
        nullptr                                         // Additional application data.
    );

    // Create graphics context.
    m_Data.GraphicsContext = CreateScope<GraphicsContext>(m_Handle, m_Data.Width, m_Data.Height);

    // Set user data and show.
    SetWindowLongPtrW(m_Handle, GWLP_USERDATA, reinterpret_cast<LPARAM>(&m_Data));
    ShowWindow(m_Handle, SW_SHOWDEFAULT);
}

uint32_t Window::GetWidth() const
{
    return m_Data.Width;
}

uint32_t Window::GetHeight() const
{
    return m_Data.Height;
}

bool Window::GetVSync() const
{
    return m_Data.VSync;
}

void Window::SetVSync(bool vsync)
{
    m_Data.VSync = vsync;
}

void Window::DispatchEvents()
{
    MSG msg;
    memset(&msg, 0, sizeof(MSG));
    while (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
    {
        // Translate virtual key message into character message.
        TranslateMessage(&msg);

        // Dispatch message to window procedure.
        DispatchMessageW(&msg);
    }
}

void Window::Update()
{
    m_Data.GraphicsContext->SwapBuffers();
}