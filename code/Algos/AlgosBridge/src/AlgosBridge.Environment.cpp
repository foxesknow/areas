#include "..\WindowsIncludes.h"

#include <AlgosBridge\AlgosBridge.Environment.h>

#include <cstring>

EnvironmentVTable s_VTable;

BRIDGE_API int32_t Register_Environment_VTable(const EnvironmentVTable *vtable)
{
    REQUIRE_OR_SET_ERROR_AND_RETURN(vtable != 0, BRIDGE_ERROR_INVALID_PARAMETER, false);
    REQUIRE_OR_SET_ERROR_AND_RETURN(vtable->StructSize != 0, BRIDGE_ERROR_INVALID_PARAMETER, false);
    
    REQUIRE_OR_SET_ERROR_AND_RETURN(s_VTable.StructSize == 0, BRIDGE_ERROR_VTABLE_ALREADY_REGISTERED, false);

    auto supportedSize = sizeof(s_VTable);
    auto suppliedSize = vtable->StructSize;

    if(suppliedSize > supportedSize)
    {
        BRIDGE_SET_ERROR(BRIDGE_ERROR_VTABLE_TOO_BIG);
        return false;
    }

    // As our vtable is bigger than the supplied one we want the extra functions to be null
    if(suppliedSize < supportedSize)
    {
        std::memset(&s_VTable, 0, sizeof(s_VTable));
    }

    std::memcpy(&s_VTable, vtable, vtable->StructSize);

    Bridge_SetLastError(BRIDGE_SUCCESS);
    return true;
}

BRIDGE_API ENVIRONMENT_TIMER_HANDLE Environment_CreateRepeatableTimer(size_t milliseconds, void *callbackData, ENVIRONMENT_TIMER_HANDLER handler)
{
    ASSERT_OR_SET_ERROR_AND_RETURN(s_VTable.StructSize != 0, BRIDGE_ERROR_VTABLE_NOT_REGISTERED, 0);
    ASSERT_OR_SET_ERROR_AND_RETURN(s_VTable.Environment_CreateRepeatableTimer != 0, BRIDGE_ERROR_VTABLE_FUNCTION_NOT_REGISTERED, 0);

    return s_VTable.Environment_CreateRepeatableTimer(milliseconds, callbackData, handler);
}

BRIDGE_API ENVIRONMENT_TIMER_HANDLE Environment_CreateSingleShotTimer(size_t milliseconds, void *callbackData, ENVIRONMENT_TIMER_HANDLER handler)
{
    ASSERT_OR_SET_ERROR_AND_RETURN(s_VTable.StructSize != 0, BRIDGE_ERROR_VTABLE_NOT_REGISTERED, 0);
    ASSERT_OR_SET_ERROR_AND_RETURN(s_VTable.Environment_CreateSingleShotTimer != 0, BRIDGE_ERROR_VTABLE_FUNCTION_NOT_REGISTERED, 0);

    return s_VTable.Environment_CreateSingleShotTimer(milliseconds, callbackData, handler);
}

BRIDGE_API int32_t Environment_CloseTimer(ENVIRONMENT_TIMER_HANDLE handle)
{
    ASSERT_OR_SET_ERROR_AND_RETURN(s_VTable.StructSize != 0, BRIDGE_ERROR_VTABLE_NOT_REGISTERED, 0);
    ASSERT_OR_SET_ERROR_AND_RETURN(s_VTable.Environment_CloseTimer != 0, BRIDGE_ERROR_VTABLE_FUNCTION_NOT_REGISTERED, 0);

    return s_VTable.Environment_CloseTimer(handle);
}