using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;

namespace ManagedLayer
{
    public delegate void MarketDataHandler(string symbol);

    public static class MarketData
    {
        private static long s_ID;

        private static readonly Dictionary<string, SubscriptionData> s_Subscriptions = new Dictionary<string, SubscriptionData>();

        private static readonly MarketDataPrintHandler s_MarketDataPrintHandler = HandlePrint;
        private static IntPtr s_MarketDataPrintHandlerPtr;
        private static GCHandle s_MarketDataPrintHandlerHandle;

        static MarketData()
        {
            s_MarketDataPrintHandlerPtr = Marshal.GetFunctionPointerForDelegate(s_MarketDataPrintHandler);
            
            // TODO: Not sure we really need this...
            s_MarketDataPrintHandlerHandle = GCHandle.Alloc(s_MarketDataPrintHandler);
        }

        private static ulong AllocateID()
        {
            var id = Interlocked.Increment(ref s_ID);
            return (ulong)id;
        }

        public static void Subscribe(string symbol, MarketDataHandler handler)
        {
            bool subscribe = false;

            if(s_Subscriptions.TryGetValue(symbol, out var data) == false)
            {
                data = new SubscriptionData();
                data.ID = AllocateID();
                s_Subscriptions.Add(symbol, data);

                subscribe = true;
            }

            data.Handlers.Add(handler);

            if(subscribe)
            {
                MarketData_Subscribe(symbol, data.ID, s_MarketDataPrintHandlerPtr);
            }
        }

        private static void HandlePrint(IntPtr handle, ulong callbackData, IntPtr print)
        {
            var items = s_Subscriptions.Where(pair => pair.Value.ID == callbackData);
            foreach(var item in items)
            {
                var symbol = item.Key;
                var data = item.Value;

                foreach(var handler in data.Handlers)
                {
                    handler(symbol);
                }
            }
        }

        private delegate void MarketDataPrintHandler(IntPtr handle, ulong CallbackData, IntPtr print);

        [DllImport("AlgosBridge.dll")]
        private static extern IntPtr MarketData_Subscribe(string symbol, ulong callbackData, IntPtr handler);

        class SubscriptionData
        {
            public ulong ID;
            public List<MarketDataHandler> Handlers = new List<MarketDataHandler>();
        }
    }
}
