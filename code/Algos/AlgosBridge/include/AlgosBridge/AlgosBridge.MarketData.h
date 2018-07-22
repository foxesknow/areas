#pragma once

#include <AlgosBridge\AlgosBridge.h>

#ifdef __cplusplus
extern "C" {
#endif

// Disable warning about empty arrays in C++ builds
#pragma warning(disable : 4200)

 DECLARE_BRIDGE_HANDLE(MARKET_DATA_SUBSCRIPTION_HANDLE);

#define MARKET_DATA_SYMBOL_LENGTH 24

typedef struct tagMarketDataQuote
{
    PriceType Price;
    QuantityType Quantity;
}MarketDataQuote;

typedef struct tagMarketDataDepthLevel
{
    MarketDataQuote Bid;
    MarketDataQuote Ask;
}MarketDataDepthLevel;

typedef struct tagMarketDataPrint
{
    size_t StructSize;

    char Symbol[MARKET_DATA_SYMBOL_LENGTH];

    MarketDataQuote Bid;
    MarketDataQuote Ask;

    size_t DepthCount;
    MarketDataDepthLevel Depth[];
}MarketDataPrint;


// A handler for a market data event
typedef void (*MARKET_DATA_PRINT_HANDLER)(MARKET_DATA_SUBSCRIPTION_HANDLE handle, uint64_t CallbackData, const MarketDataPrint *print);

BRIDGE_API MARKET_DATA_SUBSCRIPTION_HANDLE MarketData_Subscribe(const char *symbol, uint64_t callbackData, MARKET_DATA_PRINT_HANDLER handler);
BRIDGE_API int32_t MarketData_Unsubscribe(MARKET_DATA_SUBSCRIPTION_HANDLE handle);

typedef struct tagMarketDataVTable
{
    size_t StructSize;

    MARKET_DATA_SUBSCRIPTION_HANDLE (*MarketData_Subscribe)(const char *symbol, uint64_t callbackData, MARKET_DATA_PRINT_HANDLER handler);
    int32_t (*MarketData_Unsubscribe)(MARKET_DATA_SUBSCRIPTION_HANDLE handle);
}MarketDataVTable;

BRIDGE_API int32_t Register_MarketData_VTable(const MarketDataVTable *vtable);

#pragma warning(default : 4200)

#ifdef __cplusplus
}
#endif
