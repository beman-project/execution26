// Copyright © 2024 Beman Project
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// ----------------------------------------------------------------------------

#ifndef INCLUDED_TEST_STOP_TOKEN
#define INCLUDED_TEST_STOP_TOKEN

#include <beman/stop_token.hpp>
#include <test/execution.hpp>

#include <algorithm>
#include <atomic>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <ranges>
#include <thread>

// ----------------------------------------------------------------------------

namespace test
{
    template <::test_std::stoppable_token Token, typename Stop>
    auto stop_visible(Token, Stop) -> void;
    template <::test_std::stoppable_token Token, typename Stop>
    auto stop_callback(Token, Stop) -> void;
    template <::test_std::stoppable_token Token, typename Stop>
    auto stop_callback_dtor_deregisters(Token, Stop) -> void;
    template <::test_std::stoppable_token Token, typename Stop>
    auto stop_callback_dtor_other_thread(Token, Stop) -> void;
    template <::test_std::stoppable_token Token, typename Stop>
    auto stop_callback_dtor_same_thread(Token, Stop) -> void;

    template <typename MakeStopSource> auto stop_source(MakeStopSource);
}

// ----------------------------------------------------------------------------

template <::test_std::stoppable_token Token, typename Stop>
inline auto test::stop_visible(Token token, Stop stop) -> void
{
    Token tokens[] = { token, token, token, token };

    auto predicate = [](auto&& t){ return t.stop_requested(); };
    assert((::std::ranges::none_of(tokens, predicate)));
    stop();
    assert((::std::ranges::all_of(tokens, predicate)));
}

// ----------------------------------------------------------------------------

template <::test_std::stoppable_token Token, typename Stop>
inline auto test::stop_callback(Token token, Stop stop) -> void
{
    struct Data
    {
        Token         token;
        ::std::size_t count{};
        bool          stop_requested{this->token.stop_requested()};
    };

    struct Callback
    {
        Data* data;
        Callback(Data* data): data(data) {}
        auto operator()()
        {
            ++this->data->count;
            this->data->stop_requested = this->data->token.stop_requested();
        }
    };

    Data data{token};

    ::test_std::stop_callback_for_t<Token, Callback> cb0(token, &data);
    assert(data.count == 0);
    assert(data.stop_requested == false);
    stop();

    if (token.stop_possible())
    {
        assert(data.count == 1);
        assert(data.stop_requested == true);
        stop();
        assert(data.count == 1);

        ::test_std::stop_callback_for_t<Token, Callback> cb1(token, &data);
        assert(data.count == 2);
        stop();
        assert(data.count == 2);
    }
}

// ----------------------------------------------------------------------------

template <::test_std::stoppable_token Token, typename Stop>
auto test::stop_callback_dtor_deregisters(Token token, Stop stop) -> void
{
    struct Callback
    {
        bool* ptr;
        Callback(bool* ptr): ptr(ptr) {}
        auto operator()() { *this->ptr = true; }
    };

    bool flag{};
    ::std::invoke([token, &flag]{
        ::test_std::stop_callback_for_t<Token, Callback> cb(token, &flag);
        assert(flag == false);
    });

    stop();
    assert(flag == false);
}

// ----------------------------------------------------------------------------

template <::test_std::stoppable_token Token, typename Stop>
inline auto test::stop_callback_dtor_other_thread(Token token, Stop stop) -> void
{
    struct Data
    {
        ::std::atomic<bool>       thread_complete{};
        ::std::atomic<bool>       dtor_done{};
        ::std::mutex              lock;
        ::std::condition_variable cond;
        bool                      dtor_started{};
    };
    struct Callback
    {
        Data* data;
        Callback(Data* data): data(data) {}
        auto operator()() -> void
        {
            using namespace ::std::chrono_literals;
            {
                ::std::lock_guard guard(this->data->lock);
                this->data->dtor_started = true;
            }
            this->data->cond.notify_one();
            ::std::this_thread::sleep_for(1ms);
            this->data->dtor_done = true;
            assert(this->data->thread_complete == false);
        }
    };

    Data data;

    using CB = ::test_std::stop_callback_for_t<Token, Callback>;
    ::std::unique_ptr<CB> ptr(new CB(token, &data));

    ::std::thread thread([&ptr, &data]{
        {
            std::unique_lock guard(data.lock);
            data.cond.wait(guard, [&data]{ return data.dtor_started; });
        }

        ptr.reset();
        assert(data.dtor_done);
        data.thread_complete = true;
    });

    stop();

    thread.join();
}

// ----------------------------------------------------------------------------

template <::test_std::stoppable_token Token, typename Stop>
inline auto test::stop_callback_dtor_same_thread(Token token, Stop stop) -> void
{
    struct Base
    {
        virtual ~Base() = default;
    };
    struct Callback
    {
        ::std::unique_ptr<Base>* self;
        Callback(::std::unique_ptr<Base>* self): self(self) {}
        auto operator()() { this->self->reset(); }
    };
    struct Object
        : Base
    {
        ::test_std::stop_callback_for_t<Token, Callback> cb;
        Object(Token token, ::std::unique_ptr<Base>* self): cb(token, self) {}
    };
    ::std::unique_ptr<Base> ptr;
    ptr.reset(new Object(token, &ptr));

    ::std::atomic<bool> done{};
    ::std::thread thread([&done]{
        using namespace ::std::chrono_literals;
        ::std::this_thread::sleep_for(10ms);
        assert(done);
    });

    stop();
    done = true;
    thread.join();
}

// ----------------------------------------------------------------------------

template <typename MakeStopSource>
inline auto test::stop_source(MakeStopSource factory)
{
    using Source = decltype(factory());
    static_assert(::test_detail::stoppable_source<Source>);
    static_assert(::test_std::stoppable_token<decltype(factory().get_token())>);


    {
        auto source{factory()};
        ::test::stop_visible(source.get_token(), [&source]{ source.request_stop(); });
    }
    {
        auto source{factory()};
        ::test::stop_callback(source.get_token(), [&source]{ source.request_stop(); });
    }
    {
        auto source{factory()};
        ::test::stop_callback_dtor_deregisters(source.get_token(),
                                               [&source]{ source.request_stop(); });
    }
    {
        auto source{factory()};
        ::test::stop_callback_dtor_other_thread(source.get_token(),
                                                [&source]{ source.request_stop(); });
    }
    {
        auto source{factory()};
        ::test::stop_callback_dtor_same_thread(source.get_token(),
                                               [&source]{ source.request_stop(); });
    }
}

// ----------------------------------------------------------------------------

#endif // INCLUDED_TEST_STOP_TOKEN