#include "stdafx.h"

#include <AreasClientBridge\AreasClientBridge.MarketData.h>

#include <cstring>

MarketDataVTable s_VTable;

BRIDGE_API int32_t Register_MarketData_VTable(const MarketDataVTable *vtable)
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

BRIDGE_API MARKET_DATA_SUBSCRIPTION_HANDLE MarketData_Subscribe(const MarketDataSubscriptionRequest *request)
{
    ASSERT_OR_SET_ERROR_AND_RETURN(s_VTable.StructSize != 0, BRIDGE_ERROR_VTABLE_NOT_REGISTERED, 0);
    ASSERT_OR_SET_ERROR_AND_RETURN(s_VTable.MarketData_Subscribe != 0, BRIDGE_ERROR_VTABLE_FUNCTION_NOT_REGISTERED, 0);

    return s_VTable.MarketData_Subscribe(request);
}

BRIDGE_API int32_t MarketData_Unsubscribe(MARKET_DATA_SUBSCRIPTION_HANDLE handle)
{
    ASSERT_OR_SET_ERROR_AND_RETURN(s_VTable.StructSize != 0, BRIDGE_ERROR_VTABLE_NOT_REGISTERED, 0);
    ASSERT_OR_SET_ERROR_AND_RETURN(s_VTable.MarketData_Unsubscribe != 0, BRIDGE_ERROR_VTABLE_FUNCTION_NOT_REGISTERED, false);

    return s_VTable.MarketData_Unsubscribe(handle);
}