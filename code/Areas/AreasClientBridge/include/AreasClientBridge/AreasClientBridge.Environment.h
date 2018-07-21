#pragma once

#include <AreasClientBridge\AreasClientBridge.h>

#ifdef __cplusplus
extern "C" {
#endif

DECLARE_BRIDGE_HANDLE(ENVIRONMENT_TIMER_HANDLE);

typedef struct tagEnvironmentTimerRequest
{
    size_t StructSize;

    void *CallbackData;
    void (*Callback)(ENVIRONMENT_TIMER_HANDLE handle, void *CallbackData);
}EnvironmentTimerRequest;

BRIDGE_API ENVIRONMENT_TIMER_HANDLE Environment_CreateRepeatableTimer(const EnvironmentTimerRequest *request);
BRIDGE_API ENVIRONMENT_TIMER_HANDLE Environment_CreateSingleShotTimer(const EnvironmentTimerRequest *request);

BRIDGE_API int32_t Environment_CloseTimer(ENVIRONMENT_TIMER_HANDLE handle);


#ifdef __cplusplus
}
#endif

