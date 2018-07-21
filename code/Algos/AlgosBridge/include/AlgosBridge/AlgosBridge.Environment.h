#pragma once

#include <AlgosBridge\AlgosBridge.h>

#ifdef __cplusplus
extern "C" {
#endif

DECLARE_BRIDGE_HANDLE(ENVIRONMENT_TIMER_HANDLE);

// A handler for a timer event
typedef void (*ENVIRONMENT_TIMER_HANDLER)(ENVIRONMENT_TIMER_HANDLE handle, void *CallbackData);

BRIDGE_API ENVIRONMENT_TIMER_HANDLE Environment_CreateRepeatableTimer(size_t milliseconds, void *callbackData, ENVIRONMENT_TIMER_HANDLER handler);
BRIDGE_API ENVIRONMENT_TIMER_HANDLE Environment_CreateSingleShotTimer(size_t milliseconds, void *callbackData, ENVIRONMENT_TIMER_HANDLER handler);
BRIDGE_API int32_t Environment_CloseTimer(ENVIRONMENT_TIMER_HANDLE handle);

typedef struct tagEnvironmentVTable
{
    size_t StructSize;

    ENVIRONMENT_TIMER_HANDLE (*Environment_CreateRepeatableTimer)(size_t milliseconds, void *callbackData, ENVIRONMENT_TIMER_HANDLER handler);
    ENVIRONMENT_TIMER_HANDLE (*Environment_CreateSingleShotTimer)(size_t milliseconds, void *callbackData, ENVIRONMENT_TIMER_HANDLER handler);
    int32_t (*Environment_CloseTimer)(ENVIRONMENT_TIMER_HANDLE handle);
}EnvironmentVTable;

BRIDGE_API int32_t Register_Environment_VTable(const EnvironmentVTable *vtable);

#ifdef __cplusplus
}
#endif

