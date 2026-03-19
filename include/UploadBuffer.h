#pragma once

#include <cstdint>
#include <memory>

namespace TextureUpload
{
	class UploadBuffer
	{
	public:
		explicit UploadBuffer(size_t size, size_t alignment, uint32_t width, uint32_t height);
		~UploadBuffer();

		// Non-copyable, movable
		UploadBuffer(const UploadBuffer&) = delete;
		UploadBuffer& operator=(const UploadBuffer&) = delete;
		UploadBuffer(UploadBuffer&& other) = delete;
		UploadBuffer& operator=(UploadBuffer&& other) = delete;

		uint8_t* Get() noexcept { return pBuffer; }
		const uint8_t* Get() const noexcept { return pBuffer; }

		size_t Size() const noexcept { return bufferSize; }
		uint32_t Width() const noexcept { return imageWidth; }
		uint32_t Height() const noexcept { return imageHeight; }

	private:
		uint8_t* pBuffer;
		size_t bufferSize;
		uint32_t imageWidth;
		uint32_t imageHeight;
	};

	using UploadBufferPtr = std::unique_ptr<UploadBuffer>;
}