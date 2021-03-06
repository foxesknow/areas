﻿using System;

namespace ManagedLayer
{
    public class Bootstrap
    {
        public static void SubsystemMain(IntPtr subsystemHandle)
        {
            Console.WriteLine("SubsystemHandle = {0}", subsystemHandle);
            Console.WriteLine("Subscribing");

            MarketData.Subscribe("VOD.L", HandlePrint);
            MarketData.Subscribe("BARC.L", HandlePrint);

            Console.WriteLine("Subscribed");
        }

        static void HandlePrint(string symbol)
        {
            Console.WriteLine("got a print for {0}", symbol);
        }
    }
}
