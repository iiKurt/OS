#pragma once

#include <stdint.h>

#include "EFIMemory.hpp"

uint64_t GetMemorySize(void* mMap, uint64_t mMapEntries, uint64_t mMapDescriptorSize);
