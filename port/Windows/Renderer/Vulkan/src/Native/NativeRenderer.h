#pragma once

#include <cstdint>
#include <string>

#include "Pipeline.h"
#include "FrameBuffer.h"
#include "VulkanIncludes.h"

namespace Renderer
{
	struct SimpleTexture;
	struct SimpleMesh;

	namespace Native
	{
		constexpr int gWidth = 0x200;
		constexpr int gHeight = 0x200;

		struct PipelineKey {
			union {
				// 32 bit key
				struct {
					uint32_t bWireframe : 1;
					uint32_t bGlsl : 1;
				} options;

				uint32_t key{};
			};
		};

		static_assert(sizeof(uint32_t) == sizeof(PipelineKey), "PipelineKey size mismatch");

		struct PipelineCreateInfo {
			std::string vertShaderFilename;
			std::string fragShaderFilename;

			PipelineKey key;
		};

		using PipelineMap = std::unordered_map<size_t, Renderer::Pipeline>;

		void CreateRenderPass(VkRenderPass& renderPass, const char* name);
		void CreatePipeline(const PipelineCreateInfo& createInfo, const VkRenderPass& renderPass, Renderer::Pipeline& pipeline, const char* name);

		FrameBufferBase& GetFrameBuffer();

		void Setup();
		void Render(const VkFramebuffer& framebuffer, const VkExtent2D& extent);

		void BindTexture(SimpleTexture* pTexture);
		void AddMesh(SimpleMesh* pMesh);
		void PushGlobalMatrices(float* pModel, float* pView, float* pProj);
		void PushModelMatrix(float* pModel);
		void PushAnimMatrix(float* pAnim);

		const VkSampler& GetSampler();
		const VkImageView& GetColorImageView();

		double GetRenderTime();
		double GetRenderWaitTime();
		double GetRenderThreadTime();
	}
}