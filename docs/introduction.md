# Introduction to std::execution (Sender/Receiver)

C++26 adds support some support for asynchrony. In particular,
a vocabulary for dealing with asynchronous components is added by
the [`std::execution` proposal](http://wg21.link/p2300). This
document provides an introduction to using the corresponding
facilities.

## Disclaimer

This document uses some definitions which may be a more vague than
some people might desire. The intent is to provide something
comprehensible: for precise definition of the system it will be
necessary to look at the specification.

## Asynchrony

The `std::execution` system is centered around
[_asynchrony_](https://en.wikipedia.org/wiki/Asynchrony_(computer_programming)).
Here is an attempt to clarify what is meant by the term:

- A program or part thereof is _synchronous_ if it does all it operation
    without interrupting the program flow (thread of execution) to
    deal with later completions. That is quite a common approach
    to programming. Unfortunately, doing so blocks the current thread of
    execution if something isn't ready, e.g., because the next piece of
    data can't be read off a network connection.
- A program is _asynchronous_ if it avoids for threads of execution
    to block for significant amounts of time. The reason for allowing
    blocking for small amounts of time is avoid requiring wait-free
    algoritms, e.g., for adding work into a queue. Also, a thread
    may block if there is no meaningful work it could do while
    waiting for the completion of requested work. For example, when
    needing another piece of data read of a network connection the
    program would arrange for the read to happen and set up a way
    to get resumed when that is available.


The intuition is that asynchronous programs set up ways to continue
whenever something needed becomes ready instead of waiting for the
result. While the result isn't ready a thread may participate in
producing the result (if the result is some computation) or it could
work on progressing something entirely unrelated.

## std::execution: Vocabulary for Asynchrony

The key contribution of [`std::execution`](http://wg21.link/p2300)
is to provide a vocabulary for composing asynchronous work in a
flexible and efficient manner. The
[`std::execution`](http://wg21.link/p2300) concepts are the
abstractions for asychrony supporting the creation of algorithms,
data structures, and utilities in a similar way how the iterator
concepts are an abstraction for the creation of sequence algorithms.

The main user-visible abstraction is a [`sender`](#sender),
representing some sort of work. For example, a `sender` could
represent getting data from a database or executing some computation.
Senders often get composed into senders until the desired overall
work is described and the work can get get started. For `sender`s
to be composed and eventually become useful they rely on an ecosystem
of various components:

- Some `sender`s for asynchronous "work" are needed, often needing
    some form of context. For example
    - `sender`s for executing work on dedicated execution contexts.
    - `sender`s for timers which complete after some amount
	    of time has passed ([`resume_after`](#resume_after) or at
	    a specific time ([`resume_at`](#resume_at)).
    - `sender`s for dealing with I/O like sending data to the network
        or awaiting a network connection.
    - `sender`s interfacing existing libraries with support for
	    asynchronous operations like making [database requests](#postgres).
- Some ways to compose `sender`s:
    - An easy way to consume a `sender` is to `co_await` the `sender`
	    in a coroutine: except for `co_await`ing the work, the code
	    would look very similar to synchronous code. Such a
	    coroutines (likely to be named
	    [`std::lazy`](http://wg21.link/p2506)) would itself be
	    a `sender`.
    - `sender` can be composed with algorithms like [`when_all`](#when_all),
        [`bulk`](#bulk), [`then`](#then), etc. to achieve common needs.
- An important use of `sender` is executing them in different execution
    context via a [`scheduler`](#scheduler):
    - A [`system_context`](http://wg21.link/p2079) provides a general
	    execution resource, possibly balancing resources across an
	    entire system.
    - For specialized hardware like GPUs or FPGAs an interface can
        be provided to execute work represented by `sender`s using this
        hardware.
- Some ways to actually start asynchronous work and deal with its
    result and/or resources:
    - A "[scope](http://wg21.link/p3149)" to start and hold on to
        outstanding work, releasing resources whenever work completes.
    - A blocking way to start a `sender` and wait for its completion:
        [`async_wait`](#async_wait).
- An I/O context to deal with I/O and timers: essentially an abstraction
    of [`poll`](https://man7.org/linux/man-pages/man2/poll.2.html) (which
    may use other approach than `poll`)

Some parts of this ecosystem are provided by
[`std::execution`](http://wg21.link/p2300). Other parts are proposed
for standardization and/or proposals being are actively being worked
on. The aim of this document is to, at least, outline how the parts
which are not, yet, standardized can be implemented in terms of the
asynchrony vocabulary to seamlessly complement the existing ecosystem.

- TODO examples
 - TODO map/reduce
 - TODO server accessing a database
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