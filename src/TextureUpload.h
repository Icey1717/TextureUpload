#pragma once

#include <cstdint>
#include <memory>

#include "UploadBuffer.h"

namespace TextureUpload
{
	void UploadPalette(uint8_t* pSrc, uint64_t bitBltBuf, uint64_t trxPos, uint64_t trxReg, uint64_t tex0);
	UploadBufferPtr UploadTexture(uint8_t* pSrc, uint64_t bitBltBuf, uint64_t trxPos, uint64_t trxReg, uint64_t tex0);

	// Debug
	uint8_t* GetClut();
}