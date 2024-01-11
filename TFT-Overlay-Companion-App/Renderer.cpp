#include <Windows.h>
#include <iomanip>
#include <cmath>

#include "Renderer.h"
#include "Utils.h"

void Renderer::LoadFromMem(DWORD_PTR rendererBaseAddr, const std::map<std::string, uint64_t>& offsets, DWORD_PTR leagueBaseAddr, HANDLE hProcess) {
	
	if (rendererBaseAddr == 0 || leagueBaseAddr == 0) return;

	char buffer[128];
	Memory::ReadStructure(hProcess, rendererBaseAddr, buffer, sizeof(buffer), "Renderer");

	memcpy(&width, &buffer[offsets.at("RendererWidth")], sizeof(width));
	memcpy(&height, &buffer[offsets.at("RendererHeight")], sizeof(height));

	Memory::ReadStructure(hProcess, leagueBaseAddr + offsets.at("ViewProjMatrices"), buffer, sizeof(buffer), "ViewProjMatrices");

	// ViewMatrix is a 4 x 4 float matrix
	// ProjMatrix is a 4 x 4 float matrix
	// ViewProjMatrix is ViewMatrix * ProjMatrix
	memcpy(viewMatrix, &buffer, sizeof(viewMatrix));
	memcpy(projMatrix, &buffer[sizeof(viewMatrix)], sizeof(projMatrix));
	MultiplyMatrices(viewProjMatrix, viewMatrix, 4, 4, projMatrix, 4, 4);
}

void Renderer::MultiplyMatrices(float* out, float* a, int row1, int col1, float* b, int row2, int col2) {
	for (int i = 0; i < row1; i++) {
		for (int j = 0; j < col2; j++) {
			float sum = 0.f;
			for (int k = 0; k < col1; k++)
				sum = sum + a[i * col1 + k] * b[k * col2 + j];
			out[i * col2 + j] = sum;
		}
	}
}

Vector2 Renderer::WorldToScreen(const Vector3& pos) const {
	Vector2 out = {};
	Vector2 screen = { (float)width, (float)height };

	Vector4 clipCoords;
	clipCoords.x = pos.x * viewProjMatrix[0] + pos.y * viewProjMatrix[4] + pos.z * viewProjMatrix[8] + viewProjMatrix[12];
	clipCoords.y = pos.x * viewProjMatrix[1] + pos.y * viewProjMatrix[5] + pos.z * viewProjMatrix[9] + viewProjMatrix[13];
	clipCoords.z = pos.x * viewProjMatrix[2] + pos.y * viewProjMatrix[6] + pos.z * viewProjMatrix[10] + viewProjMatrix[14];
	clipCoords.w = pos.x * viewProjMatrix[3] + pos.y * viewProjMatrix[7] + pos.z * viewProjMatrix[11] + viewProjMatrix[15];

	if (clipCoords.w < 1.0f)
		clipCoords.w = 1.f;

	Vector3 M;
	M.x = clipCoords.x / clipCoords.w;
	M.y = clipCoords.y / clipCoords.w;
	M.z = clipCoords.z / clipCoords.w;

	out.x = (screen.x / 2.f * M.x) + (M.x + screen.x / 2.f);
	out.y = -(screen.y / 2.f * M.y) + (M.y + screen.y / 2.f);

	return out;
}