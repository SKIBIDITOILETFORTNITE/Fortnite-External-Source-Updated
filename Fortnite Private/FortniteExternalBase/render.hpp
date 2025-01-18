#pragma once

// Dear ImGui: standalone example application for DirectX 11

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <Windows.h>
#include <dwmapi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <TlHelp32.h>
#include <d3d11.h>
#include <d3dx11.h>
#include "settings.hpp"
#include "game.hpp"
#include "particles.hpp"
#include "ImGui/Assets/FontAwesome.hpp"
#include "ImGui/Assets/FontInter.hpp"
#include "ImGui/Assets/Assets.hpp"
#include "ImGui/Assets/ImagesBytes.hpp"

int CurrentTab = 0;
// Font Data
ID3D11Device* IDevice;
ID3D11ShaderResourceView* Logo;

// Data
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

float Clamp(const float& value, const float& min, const float& max) {
    return (value < min) ? min : (value > max) ? max : value;
}

static float menuAlpha = 0.0f;

bool Keybind2(CKeybind* keybind, const ImVec2& size_arg = ImVec2(0, 0), bool clicked = false, ImGuiButtonFlags flags = 0)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;
    // SetCursorPosX(window->Size.x - 14 - size_arg.x);
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(keybind->get_name().c_str());
    const ImVec2 label_size = ImGui::CalcTextSize(keybind->get_name().c_str(), NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    if ((flags & ImGuiButtonFlags_AlignTextBaseLine) &&
        style.FramePadding.y <
        window->DC.CurrLineTextBaseOffset)  // Try to vertically align buttons that are smaller/have no padding so that
        // text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = ImGui::CalcItemSize(
        size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    if (g.CurrentItemFlags & ImGuiItemFlags_ButtonRepeat)
        flags |= ImGuiButtonFlags_Repeat;
    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

    bool value_changed = false;
    int key = keybind->key;

    auto io = ImGui::GetIO();

    std::string name = keybind->get_key_name();

    if (keybind->waiting_for_input)
        name = "Waiting";

    if (ImGui::GetIO().MouseClicked[0] && hovered)
    {
        if (g.ActiveId == id)
        {
            keybind->waiting_for_input = true;
        }
    }
    else if (ImGui::GetIO().MouseClicked[1] && hovered)
    {
        ImGui::OpenPopup(keybind->get_name().c_str());
    }
    else if (ImGui::GetIO().MouseClicked[0] && !hovered)
    {
        if (g.ActiveId == id)
            ImGui::ClearActiveID();
    }

    if (keybind->waiting_for_input)
    {
        if (ImGui::GetIO().MouseClicked[0] && !hovered)
        {
            keybind->key = VK_LBUTTON;

            ImGui::ClearActiveID();
            keybind->waiting_for_input = false;
        }
        else
        {
            if (keybind->set_key())
            {
                ImGui::ClearActiveID();
                keybind->waiting_for_input = false;
            }
        }
    }

    // Render
    ImVec4 textcolor = ImLerp(ImVec4(201 / 255.f, 204 / 255.f, 210 / 255.f, 1.f), ImVec4(1.0f, 1.0f, 1.0f, 1.f), 1.f);

    window->DrawList->AddRectFilled(bb.Min, bb.Max, ImColor(33 / 255.0f, 33 / 255.0f, 33 / 255.0f, 1.f), 2.f);
    //window->DrawList->AddRect( bb.Min, bb.Max, ImColor( 0.f, 0.f, 0.f, 1.f ) );

    window->DrawList->AddText(
        bb.Min +
        ImVec2(
            size_arg.x / 2 - ImGui::CalcTextSize(name.c_str()).x / 2,
            size_arg.y / 2 - ImGui::CalcTextSize(name.c_str()).y / 2),
        ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_Text)),
        name.c_str());

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup |
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar;
    //ImGui::SetNextWindowPos( pos + ImVec2( 0, size_arg.y - 1 ) );
    //ImGui::SetNextWindowSize( ImVec2( size_arg.x, 47 * 1.f ) );

    {
        if (ImGui::BeginPopup(keybind->get_name().c_str(), 0))
        {

            {
                {
                    ImGui::BeginGroup();
                    {
                        if (ImGui::Selectable("hold", keybind->type == CKeybind::HOLD))
                            keybind->type = CKeybind::HOLD;
                        if (ImGui::Selectable("toggle", keybind->type == CKeybind::TOGGLE))
                            keybind->type = CKeybind::TOGGLE;
                    }
                    ImGui::EndGroup();
                }
            }

            ImGui::EndPopup();
        }
    }

    return pressed;
}

// Main code
inline std::string wstring_to_string2(const std::wstring& wstr)
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str[0], size_needed, NULL, NULL);
    return str;
}

int render()
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();

    WNDCLASSEX wc;

    wc.cbClsExtra = NULL;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = NULL;
    wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = TEXT("D3D11 Overlay ImGui");
    wc.lpszMenuName = nullptr;
    wc.style = CS_VREDRAW | CS_HREDRAW;

    ::RegisterClassEx(&wc);
    const std::string classNameAnsi = wstring_to_string2(wc.lpszClassName);

    const HWND hwnd = ::CreateWindowExA_Spoofed(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE,
        classNameAnsi.c_str(),
        "D3D11 Overlay ImGui",
        WS_POPUP,
        0, 0,
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
        nullptr,
        nullptr,
        wc.hInstance,
        nullptr
    );
    SetLayeredWindowAttributes_Spoofed(hwnd, 0, 255, LWA_ALPHA);
    const MARGINS margin = { -1, 0, 0, 0 };
    DwmExtendFrameIntoClientArea(hwnd, &margin);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(L"D3D11 Overlay ImGui", wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);


    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0, 0, 0, 0);

    // Main loop
    bool done = false;
    FrameWork::Assets::Initialize(IDevice);


    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window being minimized or screen locked
        if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
        {
            ::Sleep(10);
            continue;
        }
        g_SwapChainOccluded = false;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (GetForegroundWindow_Spoofed() == FindWindowA_Spoofed(0, "Fortnite  ") || GetForegroundWindow_Spoofed() == hwnd)
        {
            if (settings::aimbot::enable)
            {
                settings::aimbot::aimbotkey.update();

                if (settings::aimbot::aimbotkey.enabled)
                {
                    aimbot(cache::closest_pawn);
                }
            }
        }

        game_loop();

        ImGui::GetBackgroundDrawList()->AddText(ImVec2(10, 10), ImColor(255, 0, 0), "Corrupt Slotted");

        if (settings::aimbot::show_fov)
            ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), settings::aimbot::fov, ImColor(250, 250, 250, 250), 100, 1.0f);

        float alphaSpeed = 4.0f * ImGui::GetIO().DeltaTime;

        if (settings::overlay::show_menu)
            menuAlpha = Clamp(menuAlpha + alphaSpeed, 0.0f, 1.0f);
        else
            menuAlpha = Clamp(menuAlpha - alphaSpeed, 0.0f, 1.0f);

        if (menuAlpha)
        {
            //ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImGui::GetIO().DisplaySize, ImColor(0, 0, 0, static_cast<int>(200 * menuAlpha)));

           

            /*ImGuiStyle& style = ImGui::GetStyle();
            style.WindowRounding = 5.0f;
            style.FrameRounding = 3.0f;
            style.PopupRounding = 5.0f;
            style.ScrollbarRounding = 3.0f;
            style.GrabRounding = 3.0f;
            style.WindowBorderSize = 1.0f;
            style.FrameBorderSize = 1.0f;
            style.WindowPadding = ImVec2(10, 10);
            style.FramePadding = ImVec2(5, 5);*/
            ImGuiStyle* style = &ImGui::GetStyle();
            style->WindowRounding = 7;
            style->WindowBorderSize = 1;
            style->WindowPadding = ImVec2(0, 0);
            style->WindowShadowSize = 0;
            style->ScrollbarSize = 3;
            style->ScrollbarRounding = 0;
            style->PopupRounding = 5;

            // Set colors for dark theme with black and red accent
            style->Colors[ImGuiCol_WindowBg] = ImColor(14, 14, 14);      // Dark background
            style->Colors[ImGuiCol_Border] = ImColor(24, 23, 25);        // Border color (dark grey)
            style->Colors[ImGuiCol_Separator] = ImColor(255, 0, 0);       // Red separator
            style->Colors[ImGuiCol_SeparatorActive] = ImColor(255, 0, 0);
            style->Colors[ImGuiCol_SeparatorHovered] = ImColor(255, 0, 0);
            style->Colors[ImGuiCol_Tab] = ImColor(14, 14, 14);             // Dark background for tabs
            style->Colors[ImGuiCol_TabHovered] = ImColor(100, 0, 0);      // Hovered tab dark red
            style->Colors[ImGuiCol_TabActive] = ImColor(200, 0, 0);       // Active tab bright red
            style->Colors[ImGuiCol_PopupBg] = ImColor(14, 14, 14);         // Dark popup background
            style->Colors[ImGuiCol_ScrollbarBg] = ImColor(14, 14, 14);     // Scrollbar background
            style->Colors[ImGuiCol_ScrollbarGrab] = ImColor(255, 0, 0);    // Red scrollbar grab handle

            ImGui::SetNextWindowSize(ImVec2(500, 500));

            ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            {
                ImVec2 textSize = ImGui::CalcTextSize("Corrupted Services");
                ImVec2 windowSize = ImGui::GetWindowSize();
                ImGui::SetCursorPos(ImVec2((windowSize.x - textSize.x) / 2, 10));  // Center title
                ImGui::Text("Corrupted Services");

                ImVec2 lineStart = ImVec2(10, ImGui::GetCursorPosY() + 10);
                ImVec2 lineEnd = ImVec2(windowSize.x - 10, lineStart.y);
                ImGui::GetWindowDrawList()->AddLine(lineStart, lineEnd, ImColor(255, 0, 0), 2.0f); // Red line

                ImGui::Spacing();
                ImGui::Spacing();

                if (ImGui::BeginTabBar("MyTabBar")) {
                    if (ImGui::BeginTabItem("Aimbot")) {

                        ImGui::Spacing();
                        ImGui::Spacing();
                        ImGui::Checkbox(("Enabled"), &settings::aimbot::enable);
                        Keybind2(&settings::aimbot::aimbotkey, ImVec2(60, 20));
                        ImGui::Checkbox(("Show Fov"), &settings::aimbot::show_fov);

                        ImGui::Checkbox("Enable Smoothness", &settings::aimbot::enable_smoothness);

                        ImGui::SliderFloat("Smooth X", &settings::aimbot::smoothness_x, 0.0f, 30.0f);
                        ImGui::SliderFloat("Smooth Y", &settings::aimbot::smoothness_y, 1.0f, 30.0f);
                     
                        ImGui::SliderFloat(("FOV Size"), &settings::aimbot::fov, 50, 500);

                        ImGui::Combo(("Hitbox"), &settings::aimbot::aimbot_part, ("Head\0Neck\0Chest\0"));

                        
                        //ImGui::Combo("Aimbot Type", &settings::aimbot::aimbot_type, settings::aimbot::aimbot_types, IM_ARRAYSIZE(settings::aimbot::aimbot_types), IM_ARRAYSIZE(settings::aimbot::aimbot_types));
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Visuals")) {
                        ImGui::Spacing();
                        ImGui::Spacing();

                        ImGui::Checkbox(("Enabled"), &settings::visuals::enable);

                        ImGui::Checkbox(("Box"), &settings::visuals::box);
                        ImGui::Checkbox(("Skeleton"), &settings::visuals::skeleton);
                        ImGui::Checkbox(("Username"), &settings::visuals::username);
                        ImGui::Checkbox(("FOV Arrows"), &settings::visuals::FOVRadar);
                        ImGui::Checkbox(("Platform"), &settings::visuals::platform);
                        ImGui::Checkbox(("Rank"), &settings::visuals::rank);
                        ImGui::Checkbox(("Weapon"), &settings::visuals::weapon);
                        ImGui::Checkbox(("Distance"), &settings::visuals::distance);
                        ImGui::Checkbox(("SnapLines"), &settings::visuals::line);
                        ImGui::Checkbox(("Outlined"), &settings::visuals::outlines);

                        ImGui::SliderFloat("Box Thickness", &settings::aimbot::BOXthickness, 1.0f, 10.0f);
                        ImGui::SliderFloat("Skeleton Thickness", &settings::aimbot::thickness, 1.0f, 10.0f);
                        ImGui::SliderFloat("Outline Thickness", &settings::aimbot::OUTthickness, 1.0f, 10.0f);

                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Colors")) {
                        ImGui::Spacing();
                        ImGui::Spacing();
                        ImGui::Text(" Invis Color"); ImGui::SameLine();
                        ImGui::ColorPicker4("Invis Color", (float*)&settings::visuals::InVisColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
                        ImGui::Spacing();
                        ImGui::Spacing();
                        ImGui::Text(" Vis Color"); ImGui::SameLine();
                        ImGui::ColorPicker4("Vis Color", (float*)&settings::visuals::VisColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem("Misc")) {
                        ImGui::Checkbox(("Team Check"), &settings::checks::team_check);
                        ImGui::Checkbox(("Visible Only"), &settings::checks::visible_check);
                        ImGui::Checkbox(("Stream Proof"), &settings::aimbot::streamer);
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }

            }
        }

        if (GetAsyncKeyState(VK_INSERT) & 1)
            settings::overlay::show_menu = !settings::overlay::show_menu;


        if (settings::overlay::show_menu)
            SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
        else
            SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW);

        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Present
        HRESULT hr = g_pSwapChain->Present(1, 0);   // Present with vsync
        //HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
        g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(L"D3D11 Overlay ImGui", wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}