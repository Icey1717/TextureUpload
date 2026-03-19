#include "UploadBuffer.h"

TextureUpload::UploadBuffer::UploadBuffer(size_t size, size_t alignment, uint32_t width, uint32_t height)
	: pBuffer(static_cast<uint8_t*>(_aligned_malloc(size, alignment))), bufferSize(size), imageWidth(width), imageHeight(height)
{}

TextureUpload::UploadBuffer::~UploadBuffer()
{
	if (pBuffer)
	{
		_aligned_free(pBuffer);
		pBuffer = nullptr;
	}
}