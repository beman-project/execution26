#include <beman/execution26/execution.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <memory_resource>
#include <span>
#include <vector>

namespace ex = beman::execution26;

namespace
{
    template <std::size_t Size>
    struct inline_resource
        : std::pmr::memory_resource
    {
        char const* name;
        inline_resource(char const* name): name(name) {}
        std::byte  buffer[Size];
        std::byte* next{+this->buffer};

        void* do_allocate(std::size_t size, std::size_t) override {
            std::cout << "allocating from=" << this->name << ", size=" << size << "\n";
            if (size <= std::size_t(std::distance(next, std::end(buffer)))) {
                std::byte* rc{this->next};
                this->next += size;
                return rc;
            }
            return nullptr;
        }
        void do_deallocate(void*, std::size_t, std::size_t) override {
        }
        bool do_is_equal(std::pmr::memory_resource const& other) const noexcept override {
            return this == &other;
        }
    };

    template <typename Fun>
    struct allocator_aware_fun
    {
        using allocator_type = std::pmr::polymorphic_allocator<>;

        std::remove_cvref_t<Fun> fun;
        allocator_type           allocator{};

        template <typename F>
            requires std::same_as<std::remove_cvref_t<F>, std::remove_cvref_t<Fun>>
        allocator_aware_fun(F&& fun): fun(std::forward<F>(fun)) {}
        allocator_aware_fun(allocator_aware_fun const& other, allocator_type allocator = {})
            : fun(other.fun)
            , allocator(allocator)
        {
        }
        allocator_aware_fun(allocator_aware_fun&& other)
            : fun(std::move(other.fun))
            , allocator(other.allocator)
        {
        }
        allocator_aware_fun(allocator_aware_fun&& other, allocator_type allocator)
            : fun(std::move(other.fun))
            , allocator(allocator)
        {
        }

        template <typename... Args>
        auto operator()(Args&&... args) noexcept
        {
            return this->fun(this->allocator, std::forward<Args>(args)...);
        }
    };
    template <typename Fun>
    allocator_aware_fun(Fun&& fun) -> allocator_aware_fun<Fun>;

    struct allocator_env
    {
        std::pmr::polymorphic_allocator<> allocator{};
        auto query(ex::get_allocator_t) const noexcept -> std::pmr::polymorphic_allocator<> {
            return this->allocator;
        }
    };
}

auto main() -> int
{
    int values[] = { 1, 2, 3 };
    auto s{
        ex::just(std::span(values))
        | ex::let_value(allocator_aware_fun([](auto alloc, std::span<int> v){
                return ex::just(std::pmr::vector<int>(v.begin(), v.end(), alloc));
            }))
        | ex::then([](auto&& v) noexcept {
            for (auto x: v){ std::cout << x << ", "; }
            std::cout << "\n";
        })
    };

    inline_resource<1024> state_resource("state");
    ex::sync_wait(
        ex::detail::write_env(std::move(s), allocator_env{&state_resource})
    );
}
