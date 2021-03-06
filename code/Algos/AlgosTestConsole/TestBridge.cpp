#include "stdafx.h"

#include <AlgosBridge\AlgosBridge.MarketData.h>
#include <AlgosBridge\HandleFactory.h>

#include <iostream>
#include <unordered_map>
#include <thread>
#include <chrono>

uint64_t s_NextHandle = 1;

struct SubscriptionData
{
    MARKET_DATA_SUBSCRIPTION_HANDLE Handle;
    std::string Symbol;
    void *CallbackData;
    MARKET_DATA_PRINT_HANDLER Handler;
};

AlgosBridge::HandleFactory<MARKET_DATA_SUBSCRIPTION_HANDLE> s_MarketDataHandles;

std::unordered_map<std::string, SubscriptionData> s_Subscriptions;

MARKET_DATA_SUBSCRIPTION_HANDLE Impl_MarketData_Subscribe(const char *symbol, void *callbackData, MARKET_DATA_PRINT_HANDLER handler)
{
    auto handle = s_MarketDataHandles.Allocate();
    std::cout << "Subscribing to " << symbol << " with a handle of " << handle << std::endl;

    SubscriptionData data;
    data.Handle = handle;
    data.Symbol = symbol;
    data.CallbackData = callbackData;
    data.Handler = handler;

    s_Subscriptions[data.Symbol] = data;

    return reinterpret_cast<MARKET_DATA_SUBSCRIPTION_HANDLE>(handle);
}

int32_t Impl_MarketData_Unsubscribe(MARKET_DATA_SUBSCRIPTION_HANDLE handle)
{
    return 1;
}


void InitializeMarketData()
{
    MarketDataVTable vtable{0};

    vtable.StructSize = sizeof(vtable);
    vtable.MarketData_Subscribe = Impl_MarketData_Subscribe;
    vtable.MarketData_Unsubscribe = Impl_MarketData_Unsubscribe;

    Register_MarketData_VTable(&vtable);
}

void PushMarketData()
{
    using namespace std::chrono_literals;

    std::thread thread([]
    {
        for(int i = 0; i < 5; i++)
        {
            for(const auto &data : s_Subscriptions)
            {
                auto &subscriptionData = data.second;

                auto handler = subscriptionData.Handler;
                handler(0, subscriptionData.CallbackData, nullptr);        
            }

            std::this_thread::sleep_for(2s);
        }
    });

    thread.join();
}