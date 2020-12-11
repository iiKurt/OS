#pragma once

#include <stdint.h>

#include <Framebuffer.h>
#include <PSF.h>

typedef struct {
	Framebuffer* fb;
	PSF1_FONT* font;
	void* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescriptorSize;
} BootInfo;
