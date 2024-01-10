#pragma once

#include <iostream>
#include "Utils.h"
#include "Windows.h"

/// Represents the state of the games renderer
class Renderer {

public:
	int      width, height; // in-game resolution

	float    viewMatrix[16];
	float    projMatrix[16];
	float    viewProjMatrix[16];

	void     LoadFromMem(DWORD_PTR rendererBaseAddress, const std::map<std::string, uint64_t>& offsets, DWORD_PTR moduleBase, HANDLE hProcess);

	/// Converts world coordinates to screen coordinates
	Vector2  WorldToScreen(const Vector3& pos) const;

private:
	void     MultiplyMatrices(float* out, float* a, int row1, int col1, float* b, int row2, int col2);
};