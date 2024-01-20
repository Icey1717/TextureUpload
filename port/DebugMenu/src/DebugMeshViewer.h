#pragma once
#include "renderer.h"
#include "Types.h"
#include "imgui.h"

struct ed_3D_Scene;
struct edNODE;
struct ed_3d_hierarchy;
struct ed_g3d_manager;

namespace PS2 { struct GSTexEntry; }

namespace DebugMeshViewer {

	constexpr int gWidth = 0x400;
	constexpr int gHeight = 0x300;

	constexpr int gMaxAnimMatrices = 0x60;
	constexpr int gMaxStripIndex = 0x20;

	struct VertexConstantBuffer {
		struct alignas(16) PaddedIndex {
			uint32_t index{};
		};

		edF32MATRIX4 model;
		edF32MATRIX4 view;
		edF32MATRIX4 proj;
		edF32MATRIX4 animMatrices[gMaxAnimMatrices];
		PaddedIndex animStripToIndex[gMaxStripIndex][gMaxStripIndex];
	};

	// Implented via renderer backend.
	PS2::DrawBufferData<Renderer::GSVertexUnprocessed, uint16_t>& GetDrawBufferData();
	VertexConstantBuffer& GetVertexConstantBuffer();

	bool& GetUseGlslPipeline();
	bool& GetWireframe();

	PS2::GSTexEntry& GetTextureEntry();

	void ShowNodeMenu(edNODE* pNode);
	void ShowClusterMenu(ed_g3d_manager* pManager);

	void OnFrameBufferCreated(const ImTextureID& image);
}