#include "LoadFont.h"

#include <stddef.h>
#include <PSF.h>

#include "LoadFile.h"

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

PSF1_FONT* LoadPSF1Font(EFI_FILE_PROTOCOL* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
	EFI_FILE_PROTOCOL* font = LoadFile(Directory, Path, ImageHandle, SystemTable);

	if (font == NULL) {
		return NULL;
	}

	PSF1_HEADER* fontHeader;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(fontHeader), (void**)&fontHeader);
	UINTN size = sizeof(PSF1_HEADER);
	font->Read(font, &size, fontHeader);

	// Verify that the file is actually a PSF1 font
	if (fontHeader->magic[0] != PSF1_MAGIC0 ||
		fontHeader->magic[1] != PSF1_MAGIC1) { // It's not a valid font
		return NULL;
	}

	UINTN glyphBufferSize = fontHeader->charsize * 256; // number of glyphs (256)
	if (fontHeader->mode == 1) { // there are 512 glyphs
		glyphBufferSize = fontHeader->charsize * 512;
	}

	void* glyphBuffer;
	{
		font->SetPosition(font, sizeof(PSF1_HEADER));
		SystemTable->BootServices->AllocatePool(EfiLoaderData, glyphBufferSize, (void**)&glyphBuffer);
		font->Read(font, &glyphBufferSize, glyphBuffer);
	}

	PSF1_FONT* finishedFont;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_FONT), (void**)&finishedFont);
	finishedFont->psf1_Header = fontHeader;
	finishedFont->glyphBuffer = glyphBuffer;
	
	return finishedFont;
}
