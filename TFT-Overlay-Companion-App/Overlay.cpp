#include "Overlay.h"
#include <iostream>
#include <sstream>
#include <iomanip>

ID3D11Device* Overlay::dxDevice{ nullptr };
ID3D11DeviceContext* Overlay::dxDeviceContext{ nullptr };
IDXGISwapChain* Overlay::dxSwapChain{ nullptr };
ID3D11RenderTargetView* Overlay::dxRenderTargetView{ nullptr };
HWND Overlay::hWindow{ nullptr };
WNDCLASSEXW Overlay::wc{};

Overlay::Overlay() {

}

void Overlay::Init() {
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = windowProcedure;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.lpszClassName = L"Test Overlay";

    RegisterClassExW(&wc); 

    hWindow = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
        wc.lpszClassName,
        L"Test Overlay",
        WS_POPUP,
        0,
        0,  // ~30 if windowed mode, 0 if fullscreen
        1920,
        1080,
        nullptr,
        nullptr,
        wc.hInstance,
        nullptr
    );

    SetLayeredWindowAttributes(hWindow, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);

    {
        RECT clientArea{};
        GetClientRect(hWindow, &clientArea);

        RECT windowArea{};
        GetClientRect(hWindow, &windowArea);

        POINT diff{};
        ClientToScreen(hWindow, &diff);

        const MARGINS margins{
            windowArea.left + (diff.x - windowArea.left),
            windowArea.top + (diff.y - windowArea.top),
            clientArea.right,
            clientArea.bottom
        };

        DwmExtendFrameIntoClientArea(hWindow, &margins);
    }

    // create swap chain
    DXGI_SWAP_CHAIN_DESC sd{};
    sd.BufferDesc.RefreshRate.Numerator = 60U;
    sd.BufferDesc.RefreshRate.Denominator = 1U;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.SampleDesc.Count = 1U;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 2U;
    sd.OutputWindow = hWindow;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    constexpr D3D_FEATURE_LEVEL levels[2]{
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };

    D3D_FEATURE_LEVEL level{};

    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        levels,
        2,
        D3D11_SDK_VERSION,
        &sd,
        &dxSwapChain,
        &dxDevice,
        &level,
        &dxDeviceContext
    );

    ID3D11Texture2D* backBuffer{ nullptr };
    dxSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

    if (backBuffer) {
        dxDevice->CreateRenderTargetView(backBuffer, nullptr, &dxRenderTargetView);
        backBuffer->Release();
    }
    else {
        //return 1;
    }

    ShowWindow(hWindow, SW_SHOW);
    UpdateWindow(hWindow);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hWindow);
    ImGui_ImplDX11_Init(dxDevice, dxDeviceContext);

}

void Overlay::Update() {
    /*
        1. Should take a memory snapshot (haven't created this class yet) that contains list of minions, traits, and shop units 
        2. If minions, traits, or shops are not null, call their respective draw functions
    */
}

void Overlay::StartFrame() {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (msg.message == WM_QUIT) {
            Overlay::CleanUp();
            std::cout << "Done cleaning up\n";
            return;
        }
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void Overlay::RenderFrame() {
    ImGui::EndFrame();
    ImGui::Render();

    constexpr float clearColor[4]{ 0.f, 0.f, 0.f, 0.f };
    dxDeviceContext->OMSetRenderTargets(1, &dxRenderTargetView, nullptr);
    dxDeviceContext->ClearRenderTargetView(dxRenderTargetView, clearColor);

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    dxSwapChain->Present(1U, 0U);
}

void Overlay::CleanUp() {
    ImGui_ImplWin32_Shutdown();
    ImGui_ImplDX11_Shutdown();

    ImGui::DestroyContext();

    if (dxSwapChain) {
        dxSwapChain->Release();
    }
    if (dxDeviceContext) {
        dxDeviceContext->Release();
    }
    if (dxDevice) {
        dxDevice->Release();
    }
    if (dxRenderTargetView) {
        dxRenderTargetView->Release();
    }

    DestroyWindow(hWindow);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);
}

// create a Window Procedure to receive window messages
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK Overlay::windowProcedure(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWindow, message, wParam, lParam)) {
        return true;
    }

    if (message == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWindow, message, wParam, lParam);
}

void Overlay::DrawDot() {
    ImGui::GetBackgroundDrawList()->AddCircleFilled({ 100, 100 }, 4.f, ImColor(1.f, 0.f, 0.f));
}

void Overlay::DrawBoxAroundUnit(const std::shared_ptr<GameObject>& gameObject) {
    ImGui::GetBackgroundDrawList()->AddRect({ gameObject->box.corner1.x, gameObject->box.corner1.y }, { gameObject->box.corner2.x, gameObject->box.corner2.y }, ImColor(1.f, 0.f, 0.f));

    std::stringstream stream;
    stream << std::hex << gameObject->address;
    DrawTextAt({ gameObject->box.corner1.x, gameObject->box.corner1.y - 20.f }, stream.str());

    DrawTextAt({ gameObject->box.corner1.x, gameObject->box.corner1.y }, "1(" + std::to_string(gameObject->box.corner1.x) + ", " + std::to_string(gameObject->box.corner1.y) + ")");
    
    DrawTextAt({ gameObject->box.corner2.x, gameObject->box.corner2.y }, "2(" + std::to_string(gameObject->box.corner2.x) + ", " + std::to_string(gameObject->box.corner2.y) + ")");

}

void Overlay::DrawMinions(MemSnapshot& snapshot) {

    DrawTextAt({ 10, 10 }, "Number of units on screen: " + std::to_string(snapshot.units.size()));
    DrawTextAt({ 10, 30 }, "Number of traits on screen: " + std::to_string(snapshot.traits.size()));
    DrawTextAt({ 10, 50 }, "Number of shop units: " + std::to_string(snapshot.shopUnits.size()));
    DrawTextAt({ 10, 70 }, "Stage " + std::to_string(snapshot.stage));
    DrawTextAt({ 10, 90 }, "Round " + std::to_string(snapshot.round));
    //DrawTextAt({ 10, 110 }, "Is Combat Phase " + std::to_string(snapshot.isCombatPhase));
    DrawTextAt({ 10, 110 }, "Camera is facing: " + std::to_string(snapshot.cameraDirection));

    //DrawTextAt({ 1000, 10 }, "Shop Slot 1: " + snapshot.shopUnits.at(0)->unitName + " - " + std::to_string(snapshot.shopUnits.at(0)->cost));

    float x = 50.f;
    float y = 130.f;
    for (const auto& gameObject : snapshot.units) {
        //std::string str1 = gameObject->name + " " + std::to_string( gameObject->baseHealth);
        //DrawTextAt({x, y }, str1);
        //y += 10.f;
        DrawBoxAroundUnit(gameObject);
        //DrawTextAt({ x, y }, gameObject->name);
    }

    for (const auto& traitObject : snapshot.traits) {
        std::string str1 = traitObject->traitName + " - " + std::to_string(traitObject->count);
        DrawTextAt({ x, y }, str1);
        y += 10.f;
        //DrawTextAt({ x, y }, gameObject->name);
    }

    y = 10.f;
    for (const auto& shopUnitObject : snapshot.shopUnits) {
        std::string str1 = shopUnitObject->unitName + " - " + std::to_string(shopUnitObject->cost);
        DrawTextAt({ 400, y }, str1);
        y += 10.f;
        //DrawTextAt({ x, y }, gameObject->name);
    }
}

void Overlay::DrawTextAt(Vector2 screenPos, std::string text) {
    ImGui::GetBackgroundDrawList()->AddText({screenPos.x, screenPos.y}, ImColor(1.f, 1.f, 1.f), text.c_str());
}
