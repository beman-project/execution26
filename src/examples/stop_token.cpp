// Copyright © 2024 Beman Project
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/stop_token.hpp>
#include <condition_variable>
#include <iostream>
#include <latch>
#include <mutex>
#include <thread>

namespace exec = beman::cpp26;

// Live demo: https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGe1wAyeAyYAHI%2BAEaYxCAArKQADqgKhE4MHt6%2BekkpjgJBIeEsUTHxdpgOaUIETMQEGT5%2BXLaY9nkM1bUEBWGR0XG2NXUNWc0KQ93BvcX9sQCUtqhexMjsHAD06wDUHgkAnsR4wAgEWyZuZiYAggCcW2YaZpJbFpgshlvKxKjaFQQmGiumy2QmUABEABoAWiCqwYSihAEksII8DR%2BlsrgkmMgEJgoWYAHQaLYAdURABUABJbAIBABqAFkoZhVKsEu0AVcueYAMzBZDeLBnXluE4EBIKECbYhMADuhOAhAQXgiXiUK1kjAIhLQLHW%2BEwBHexAA1l5MAhaOsou8GCzVBUvO11u9gusBULMDa3oZ1uNUAkAPpEU2MQkIBIJEy87A8sz8hiCrzCmNuNAMfDtIMAN1qeCYEXoMbjgL5npTmBFbic42ImFYJfjieTqdFLGdrKbZYTFbbbgICHrTHQ3e5gOYbAU2NWW1ZFRFYK2tsMIBQUbMADYY1Yy4DgRS8VsAwktqHGFtBUwFEoFFtalX1Zh0GfUJfDKtaFs5SRTYSttgOIIFywInq%2BYYMFs7x7MuVYZiEDjPq%2B97HkQp4KEsKxVgI34IHguIgdsohJq0BjtO%2BkFRFs9YAI4WuMSHqsEwDHphqyErR9EEEGJ4QHMhKEVsh5VmB56QfWVBtHecp4oO0SoYG37XtRmB0ZgDEvjhhh7IJGHLOxhGCcJ9b3iZBA/ls%2BBUFQ0TalsITPneOEIKgcpnkexGiLQZFpFseAKIJwT4KIBDPiAglQpilQ5lWP5mqQc6KoSCXoKgzEofQRiDjBeoJM6TDtKQglbN%2BSy0JpyCiDeaRiLQOV4sgpruVW9YKF4tAEHeD6oXQX4OVgo77tsJWGC%2BhYkKccmXqgLB5TU5Fon5pz%2BfZqA6kJb6paxCVycVWxUF4SbkT%2B7Uvk%2B4HhieQacepoXoHxuH4QgWx7HgrToAFQ0lWexAWlsAizieCTpXKyk3RpAlfZFiLMNFsW/glmBJQlGFsIOINMKkRj7SQfkMHN%2B3fCwe1MFsT7EP9FOkyE5m/tNDAIQtXUKPQmCnqNFFgBw/xfSVw4YcwRb1RUTUyUaeIU0DGN3uDd3/jDDEjkVvMoV5tARDiTXAEaMuYEqDH1i%2BMlPVs2udT9Rza4buGMHtUvYytsvPv%2Bh4rWrGuNRRzWQZ5rR7VNcV/kJHm1R7TWO3r/mhdbEQ5cOjjY4Ye2oBEvwOGTKTY1NYlc3e7uayGewJFWHNYPW%2BvR8%2Be1i5B6NOanfx%2BXeWB1qgezO4ZX3CWeFlPlCVXqaZIkuXKkHBM1%2B2HZUAhdXDD2jYFsOODFD2tcXcN1SAzUwd1v2Qcpg7Dh9hMzVBmMMHxglMYnF37zeqDIAWd3fsqKFgXpWH/gA8iRy5rcBX1D4NmPt1cYXQkL2xYhHNSGl7yZgnkAkcn0gTbG6toNKIR0CQ3HCgoSX8wRfylEZI8rJWAJHoF7KiKRZq0DRG9TSv8wGZlqC%2BWhERZTEBgngGhbxtQFTSIJEKuIoReASC7EhqgyEUOUitcYvUtgJG%2BMXYgdVjwjzvOjIwChsHAkiq7O8pCeFqLKi%2BYc3kYLnXGOgNcZFcTLmdCjEcdUdoIAKntVK6kGBc1OEoN4yEqJHzoDBRghZ6AMK2IyHEX8hA6O2JFKxa5FHBGWgoLx3N7x5joKEqsmAYqcLFvWBK7d1oBCNLnH6O9jzcPIZgPa5CcSWk8GXbBYDHDIC2GuBJIAOyhVUH5VAQZaAP1NDuLkOY0oviSYICAXS6zMVzG9NyvSCAJSYM6VA5htxbkJDsuYPIADsu4rjfVQtYkAQzGpBmAF4FhJzTY3OIPdJwgzhl7N5Eck5nSCBnLQM6asaYzysn%2BO8g5YIeSAlCjQgqVY0wECLiEtgG0IJjjWUQe8c8KSoAgrfOYWwoQli2OMvAg1uSHK5Cc1Fb48AmEORodqtAaVgpBYCE5xsKEQAYGtW%2BhIrpO3unMPZLLvo0o%2BXckqwItqB3JWKs4lhrDUuZcc4V%2BywVCpKlMggEBzBmBxMvKsiCXypRCFvEVjKTCxDcAwbVWwEp4DeUcs1e4riQvqaFascLi6TirJi5FsYuSUrxrqvAK8fUXjEri/FsZCUTIOaKkqXyfkCCzGkXM%2BZsn01HIqk5dYvDpzcKHTW0qSqmrVXchNa4MzJoEKmw42SABUGbRmlpOfm7yYcIDlpQEm1I1a8y1qLJgBtla5hb0rRAYdZwyUqqLXcgNgZogFRIHxB6kbsDRuJTOk5JalUyu%2BpW1dnLHBUD2EGAQmBL5ZplY6ndk6mVxrnI6ZAXzAxBnzpc/gxAQxplDQwBKrb1aFqjcgCIEAxIJU2cOptN7O09NZFsC5IzFUzs7YdPAakXme2uSwiACH7UzsrYSUGhBoAPPQOB2IFgxLmrBYclSBBli1yxZdNC11VJcWfBeiwt68Olo1VqswZhghBpiu5I%2Blkz3mstdqnjpLVU4IDW6C%2BEaCXJNjTO%2BcT6QBXQ/oDNitTL3xq098tcBr0WavNVYLc1GRVmeDeenTSMzYhiY0p%2B13GoOfKM2c0zQmHBaoo5s6ztHfN6pmXpxURpnMQT4m5xlMmZ18e1Qau8YC6hVwtVagTMns3hdljxNCnGEuHGmUlocwC84flIulqTWWoMzt1YSdBwRCulpC41jBLWb2JYE0a2pGXpN1ZVRwBYtBOCxF4H4bgvBUCcDcNYawrF9IwoTDwUgBBNDDYWKaOIXBCRmFiDcfZm4NCHceBoKQzRRscEkLwFgEgNAaFIJNrQpAZscF4FKR762OBaAWHAWAMBEBdtmnQaI5BKC5VBzEYAqSmCShcismgHVohSggBEDbpAIhCc4ZwVbWPmCcK/g3BwuPeB6jYIIH%2Bm8fu8CwGqYA/6pRTdIFgO0wBxA05Z3gescMmcvY052DHyTWgY7YRwvYHgsAY4IIcO7zO8kRGSJgMEvojC0KMBthYkkmAw/pIsr%2BnrScyEECIMQ7ApDG/kEoNQGPdCXcMMYeblh9B4AiFKWAXqvMadIHkkAMvDqmgFa9jkaQmdQisTGMEpg5WWAeHiwh028mHBbvABY5Rp5%2BAgK4EYTRSCBCmEUEo2Rkg9vSJ4RoxfchpB6IX/oYxWh/CqBMHPeh0/tE6HUGvfQYhjGb%2BX0Ygwuhd5mD3tPemJAjbGxNjHb2tiqAABybihJuZ4wBKpbFA3vQPG/cCEFxnyLgcxeDfd%2B1tkAvJeSEgv9fm/t/9CcBu6QO7sRHvPem5wD7IAvua9IP9oHSwJQHEKBx0ZpgZ6BiBQhWA1gF8l8V9TZ19N8A8j888swSBiU9B%2BATcvJzdpBMCrcVB1BOc7dSA5RZQEgf8rtxsnsZ9OAv5nQCZUAqA59F9l9V8ED/cGBt8IAPAQdwDZVeRD9j9NcFg8QRx%2Bg%2BJ79rtbs4hX8aD3tbAv81thDSBttJB59CR9lJBHheQbhHhDsbh59Yh59JDeRp9Oc3shCacg8rszAzCXsLClCrCFg8kUhnBJAgA%3D%3D%3D
// The stop token classes are used to cancel work. Each
// stop token may be connected to a stop source on which
// cancellation can be requested using source.request_stop().
// The stop token reflects whether stop was requested on any
// source.
//
// There are two different needs on how the cancallation is
// indicated:
// - Active work, e.g., doing a lengthy computation,
//   would occassionally check the results are still needed
//   and abort the computation if it is not. To do so, the
//   function would use token.stop_requested() which yields
//   true once stopping was requested.
// - Inactive work, e.g., something waiting for input from
//   a user or a network connection is asleep and can't
//   reasonably check whether stopping was requested. Instead,
//   a callback gets registered which gets triggered when
//   stopping is requested. This callback can then cancel
//   the work. The callback is registered by creating an
//   object using the token's callback_type and deregistered
//   when this object is destroyed.
//
// The two use-cases are shown in the functions active() and
// inactive() respectively: they are run as threads from main()
// using a token associated with a stop source. Once both
// threads are started stopping is requested and the threads
// are joined.

// TODOs:
// The example can be simplified once standard library implementation
// catch-up. The example as is still properly shows things.
// - This example should really use std::latch but, sadly, that
//   doesn't seem to be readily enabled on MacOS.
// - std::print isn't available everywhere, yet. Let's try a simple
//   placeholder.
static ::std::mutex io_lock;
void print(std::string_view text, auto&&...)
{
    std::lock_guard     guard(io_lock);
    ::std::cout << text;
}

template <typename Token>
auto active(Token token) -> void
{
    auto i{0ull};
    while (not token.stop_requested())
    {
        // do work
        ++i;
    }
    print("active thread done: {}\n" , i);
}

template <typename Token, typename Callback>
struct stop_callback_for_t
{
    exec::stop_callback_for_t<Token, Callback> cb;
    stop_callback_for_t(Token const& token, Callback callback)
        : cb(token, callback)
    {
    }
};

#ifdef __cpp_lib_latch
template <typename Token>
auto inactive(Token token) -> void
{
    ::std::latch        latch(1);
    stop_callback_for_t cb(token, [&latch]{ latch.count_down(); });

    latch.wait();
    print("inactive thread done\n");
}
#else
template <typename Token>
auto inactive(Token token) -> void
{
    ::std::condition_variable cond;
    stop_callback_for_t cb(token, [&cond]{ cond.notify_one(); });

    ::std::mutex lock;
    ::std::unique_lock guard(lock);
    cond.wait(guard, [token]{ return token.stop_requested(); });
    print("inactive thread done\n");
}
#endif

auto main() -> int
{
    exec::stop_source source;
    ::std::thread act([&]{ active(source.get_token()); });
    ::std::thread inact([&]{ inactive(source.get_token()); });

    print("threads started\n");
    source.request_stop();
    print("threads cancelled\n");

    act.join();
    inact.join();
    print("done\n");
}