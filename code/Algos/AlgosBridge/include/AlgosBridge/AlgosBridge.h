#pragma once

#include <stdint.h>

#ifdef __cplusplus
    #ifdef ALGOSBRIDGE_EXPORTS
        #define BRIDGE_CLASS __declspec(dllexport)
    #else
        #define BRIDGE_CLASS __declspec(dllimport)
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ALGOSBRIDGE_EXPORTS
    #define BRIDGE_API extern "C" __declspec(dllexport)    
#else
    #define BRIDGE_API extern "C" __declspec(dllimport)    
#endif

#define DECLARE_BRIDGE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name

typedef uint32_t BRIDGE_ERROR_CODE;

typedef double PriceType;
typedef int64_t QuantityType;

#define BRIDGE_SUCCESS 0

#define BRIDGE_SUBSYSTEM_CORE 1
#define BRIDGE_SUBSYSTEM_MARKETDATA 2

#define MAKE_BRIDGE_ERROR(subsystem, errorCode) ((subsystem << 16) | errorCode)

#define BRIDGE_ERROR_INVALID_PARAMETER                  MAKE_BRIDGE_ERROR(BRIDGE_SUBSYSTEM_CORE, 1)
#define BRIDGE_ERROR_VTABLE_ALREADY_REGISTERED          MAKE_BRIDGE_ERROR(BRIDGE_SUBSYSTEM_CORE, 2)
#define BRIDGE_ERROR_VTABLE_TOO_BIG                     MAKE_BRIDGE_ERROR(BRIDGE_SUBSYSTEM_CORE, 3)
#define BRIDGE_ERROR_VTABLE_NOT_REGISTERED              MAKE_BRIDGE_ERROR(BRIDGE_SUBSYSTEM_CORE, 4)
#define BRIDGE_ERROR_VTABLE_FUNCTION_NOT_REGISTERED     MAKE_BRIDGE_ERROR(BRIDGE_SUBSYSTEM_CORE, 5)
#define BRIDGE_ERROR_LIBRARY_NOT_LOADED                 MAKE_BRIDGE_ERROR(BRIDGE_SUBSYSTEM_CORE, 6)
#define BRIDGE_ERROR_FUNCTION_NOT_FOUND                 MAKE_BRIDGE_ERROR(BRIDGE_SUBSYSTEM_CORE, 7)
#define BRIDGE_ERROR_SUBSYSTEM_DIDNT_START              MAKE_BRIDGE_ERROR(BRIDGE_SUBSYSTEM_CORE, 8)

BRIDGE_API void Bridge_SetLastError(BRIDGE_ERROR_CODE errorCode);
BRIDGE_API void Bridge_SetLastErrorEx(BRIDGE_ERROR_CODE errorCode, int32_t lineNumber, const char *filename);

#define BRIDGE_SET_ERROR(errorCode) Bridge_SetLastErrorEx(errorCode, __LINE__, __FILE__)

#define REQUIRE_OR_RETURN(condition, returnCode) do{if(!(condition)) return returnCode;}while(0)
#define REQUIRE_OR_SET_ERROR_AND_RETURN(condition, errorCode, returnCode) do{if(!(condition)) {BRIDGE_SET_ERROR(errorCode);  return returnCode;}}while(0)

#ifdef _DEBUG
    #define ASSERT_OR_RETURN(condition, returnCode) REQUIRE_OR_RETURN(condition, returnCode)
    #define ASSERT_OR_SET_ERROR_AND_RETURN(condition, errorCode, returnCode) REQUIRE_OR_SET_ERROR_AND_RETURN(condition, errorCode, returnCode)
#else
    #define ASSERT_OR_RETURN(condition, returnCode)
    #define ASSERT_OR_SET_ERROR_AND_RETURN(condition, errorCode, returnCode)
#endif

#ifdef __cplusplus
}
#endif