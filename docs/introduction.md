# Introduction to std::execution (Sender/Receiver)

C++26 adds support for asynchrony. In particular,
a vocabulary for dealing with asynchronous components is added by the [`std::execution` proposal](http://wg21.link/p2300). Thisdocument provides an introduction to using the corresponding
facilities.

## Disclaimer

This document uses definitions which may be more vague than
people might desire. The intent is to provide something
comprehensible: for precise definition of the system it will need to be look up in the specification.

## Asynchrony

The `std::execution` system is centered around
[_asynchrony_](https://en.wikipedia.org/wiki/Asynchrony_(computer_programming)).
Here is an attempt to clarify what is meant by the term:

- A program or part thereof is _synchronous_ if it does all its operations
    without interrupting the program flow (thread of execution) to deal with later completions. That is a common approach to programming. Unfortunately, doing so blocks the current thread of execution if something isn't ready, e.g., because the next piece of data can't be read off a network connection.
- A program is _asynchronous_ if it avoids for threads of execution to block for significant amounts of time. The reason for allowing blocking for small amounts of time to is avoid requiring wait-free algoritms, e.g., for adding work into a queue. Also, a thread may block if there is no meaningful work it could do while waiting for the completion of requested work. For example, when needing another piece of data read of a network connection the program would arrange for the read to happen and set up a way to get resumed when that is available.

The intuition is that asynchronous programs set up ways to continue later without blocking when  something needed becomes ready instead of waiting for the result. While the result isn't ready a thread may participate in producing the result (if the result is a computation) or it could work on progressing something entirely unrelated.

## std::execution: Vocabulary for Asynchrony

The key contribution of [`std::execution`](http://wg21.link/p2300) is to provide a vocabulary for composing asynchronous work in a flexible and efficient manner. The [`std::execution`](http://wg21.link/p2300) concepts are the abstractions for asychrony supporting the creation of algorithms, data structures, and utilities in a similar way how the iterator
concepts are an abstraction for the creation of sequence algorithms.

The main user-visible abstraction is a [`sender`](#sender),
representing workto be executed. For example, a `sender` could
represent getting data from a database or executing a computation. Senders get composed into senders until the desired complete work is described and the work can get get started. For `sender`s to be composed and eventually become useful they rely on an ecosystem of various components:

- `sender`s for asynchronous "work" are needed. For example
    - `sender`s for executing work on dedicated execution contexts like a thread pool or specialized hardware.
    - `sender`s for timers which complete after some amount of time has passed ([`resume_after`](#resume_after) or at a specific time ([`resume_at`](#resume_at)).
    - `sender`s for dealing with I/O like sending data to the network or awaiting a network connection.
    - `sender`s interfacing existing libraries with support for asynchronous operations like making [database requests](#postgres).
- Algorithms and components to compose `sender`s:
    - An easy way to consume a `sender` is to `co_await` the `sender` in a coroutine: except for `co_await`ing the work, the code would look very similar to synchronous code. A `sender`-aware coroutine (likely to be named
	    [`std::lazy`](http://wg21.link/p2506) for a standard library provided one) would itself be a `sender`.
    - `sender`s can be composed with algorithms like [`when_all`](#when_all), [`bulk`](#bulk), [`then`](#then), and many more to achieve common needs.
- An important use of `sender`s is executing them in different execution contexts via [`scheduler`](#scheduler)s:
    - A [`system_context`](http://wg21.link/p2079) provides a general execution resource, balancing resources across an
	    entire system.
    - For specialized hardware like GPUs or FPGAs an interface can be provided to execute work represented by `sender`s using this hardware.
- Ways to start asynchronous work and deal with its result and/or resources:
    - A "[scope](http://wg21.link/p3149)" to start and hold on to outstanding work, releasing resources when work completes.
    - A blocking way to start a `sender` and wait for its completion: [`async_wait`](#async_wait).
- An I/O context to deal with I/O and timers: essentially an abstraction of [`poll`](https://man7.org/linux/man-pages/man2/poll.2.html) (which may use other approach than `poll`)

The [`std::execution`](http://wg21.link/p2300) proposal provides a small selection of important parts of this ecosystem. Other parts are proposed for standardization and/or proposals are actively being worked on. This document, at least, tries to outline how important missing parts which are not, yet, standardized can be implemented in terms of the asynchrony vocabulary to seamlessly complement the existing ecosystem.

## Examples

Using asynchrony based on `std::execution` doesn’t need an understanding of the detailed concept used to implement it! This section provides examples of how a few typical asynchronous approaches can be implemented. Later sections provide the details and outline how the necessary components can be implemented. The examples are using easy approaches and could be improved to be more efficient or more generic which would, however, make them less assessible. Later sections may provide refinements with corresponding refinements. The [`std::execution` proposal](http://wg21.link/p2300) does contain a set of examples which do use lower-level tools: production code _may_ end up more similar to these but they are unnecessarily complex for an introduction.

The examples use components from a namespace `intro` containing a few tools achieving to demonstrate basic functionality. The entire purpose of these tools is to create meaningful examples or to provide components which are expected to get standardized but don’t even have a proposal, yet. The implementations are in the [Appendix](#appendix) below.

### Hello Asynchronous World
Writing `”Hello, world!”` asynchronously doesn’t show much. Instead, let’s have a program

    int main()
    {
       // A tool to emit a string upon destruction:
       using on_exit = std::unique_ptr<char const*,
           decltype([](auto m){
               std::cout << "exiting: " << m << "\n"; })>;
       
       // Functions running an infinite loop sending a message:
       auto num = [](intro::channel& c)->ex::task<>{
            on_exit clean_up("num");
            for (;;) std::cout << (co_await c) << "\n";
        }, channels[0]);
        auto msg = [](intro::channel& c, auto m)->ex::task<> {
           on_exit clean_up(m);
           for (;;){ co_await c; std::cout << m << "\n"; }
        };
        
        // Set up asynchronous work:
        intro::channel channels[4];
        ex::counting_scope scope;
        scope.spawn(num(channels[0]));
        scope.spawn(msg(channels[1]), "fizz");
        scope.spawn(msg(channels[2]), "buzz");
        scope.spawn(msg(channels[3]), "fizzbuzz");

        // Resume the functions as appropiate:
        for (int i = 1; i != 100; ++i)
        {
            channels[2 * (i % 5 == 0) + (i % 3 == 0)].send(i);
        }
        
        // Finally, stop the system and clean-up:
        scope.get_stop_token().request_stop();
        ex::sync_wait(scope.on_empty());
    }

### Map/Reduce

The purpose of map/reduce is to distribute (“map”) work to many workers and then collect the results of the many workers into a result. It is one of the fundamental building blocks of concurrent programmings. Here is a simple implementation:

    template <typename Range, typename Fun, typename Combine>
    task<typename Combine::value_type>
    map_reduce(Range const& range, Fun fun, Combine combine)
    {
        using value_t = typename Fun::value_type;
        using result_t = typename Combine::value_type;
        
        std::vector<value_t> aggregate(size(range));
        co_await ex::bulk();
        return std::accumulate(aggregate, combine, result_t()); 
    }
### Simple Web Server
 TODO server accessing a database
 - TODO …
- TODO inner workings
 - TODO sender/receiver abstraction and contract
- TODO implementation
 - TODO algorithm: when_any
 - TODO data structure: async_scope
 - TODO task coroutine
 - TODO io_context
 - TODO Postgres binding
 - TODO simple thread pool