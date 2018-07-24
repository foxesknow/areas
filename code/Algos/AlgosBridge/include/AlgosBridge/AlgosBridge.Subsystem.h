#pragma once

#include <AlgosBridge\AlgosBridge.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SUBSYSTEM_API extern "C" __declspec(dllexport)

DECLARE_BRIDGE_HANDLE(SUBSYSTEM_HANDLE);

typedef int32_t (*START_ADAPTER_ENVIRONMENT)(SUBSYSTEM_HANDLE);

BRIDGE_API int32_t Subsystem_Install(const char *libraryName);

#ifdef __cplusplus
}
#endif

