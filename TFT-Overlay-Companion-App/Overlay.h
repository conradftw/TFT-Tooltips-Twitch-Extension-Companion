#pragma once

#include <Windows.h>
#include <string>
#include <set>
#include <vector>
#include <memory>

#include <d3d11.h>
#include <dwmapi.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

#include "Utils.h"
#include "GameObject.h"
#include "TraitObject.h"
#include "MemSnapshot.h"

class Overlay {

public:
	Overlay();
	void Init();
	void StartFrame();
	void Update();
	void RenderFrame();
	void CleanUp();
	void DrawDot();
	void DrawNumberAtCorner(int num);
	void DrawTextAt(Vector2 screenPos, std::string text);
	void DrawMinions(MemSnapshot& snapshot);
	void DrawBoxAroundUnit(const std::shared_ptr<GameObject>& gameObject);

private:
	static LRESULT CALLBACK windowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	bool isWindowVisible = true;
	bool isOverlayVisible = true;
	static HWND hWindow;
	static WNDCLASSEXW wc;

	// DirectX stuff
	static ID3D11Device* dxDevice;
	static ID3D11DeviceContext* dxDeviceContext;
	static IDXGISwapChain* dxSwapChain;
	static ID3D11RenderTargetView* dxRenderTargetView;

};