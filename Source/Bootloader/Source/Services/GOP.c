#include "GOP.h"

#include <stddef.h>

#include "../EFI/Statuses.h"

Framebuffer framebuffer;

Framebuffer* InitializeGOP(EFI_SYSTEM_TABLE* SystemTable) {
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_STATUS status;

	status = SystemTable->BootServices->LocateProtocol(&gopGuid, NULL, (void**)&gop);
	if(status == EFI_ERROR) {
		return NULL;
	}

	framebuffer.BaseAddress = (void*)gop->Mode->FrameBufferBase;
	framebuffer.BufferSize = gop->Mode->FrameBufferSize;
	framebuffer.Width = gop->Mode->Info->HorizontalResolution;
	framebuffer.Height = gop->Mode->Info->VerticalResolution;
	framebuffer.PixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;

	return &framebuffer;
}