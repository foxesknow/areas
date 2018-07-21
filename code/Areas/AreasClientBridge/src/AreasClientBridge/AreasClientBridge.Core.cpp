#include "stdafx.h"

#include <AreasClientBridge\AreasClientBridge.h>

#include <thread>

struct ErrorDetails
{
    BRIDGE_ERROR_CODE ErrorCode;
    int32_t LineNumber;
    const char *Filename;
};

thread_local ErrorDetails s_ErrorDetails;

BRIDGE_API void Bridge_SetLastError(BRIDGE_ERROR_CODE errorCode)
{
    s_ErrorDetails.ErrorCode = errorCode;
    s_ErrorDetails.LineNumber = 0;
    s_ErrorDetails.Filename = 0;
}

BRIDGE_API void Bridge_SetLastErrorEx(BRIDGE_ERROR_CODE errorCode, int32_t lineNumber, const char *filename)
{
    s_ErrorDetails.ErrorCode = errorCode;
    s_ErrorDetails.LineNumber = lineNumber;
    s_ErrorDetails.Filename = filename;
}