# Introduction to std::execution (Sender/Receiver)

C++26 adds support some support for asynchronicity. In particular,
a vocabulary for dealing with asynchronous components is added by
the [`std::execution` proposal](http://wg21.link/p2300). This
document provides an introduction to using the corresponding
facilities.

## Disclaimer

This document uses some definitions which may be a more vague than
some people might desire. The intent is to provide something
comprehensible: for precise definition of the system it will be
necessary to look at the specification.

- TODO Asynchronicity
   - TODO synchronous: never waits. Reading an atomic: OK, waiting for a value: not OK
   - TODO asynchronous: anything which may wait for something
   - TODO intuition: do something else while waiting
- TODO std::execution: vocabulary to deal with asynchronicity
   - TODO like STL iterator concepts and a few fundamental algorithms
   - TODO enables creation of asynchronous algorithms, data structures, and tools
   - TODO support composing of asynchronous work
- TODO Asynchronous ecosystem: can built using the vocabulary and should/will be standardized in the future
   - TODO asynchronous work: sender (monad?)
      - TODO lazy: the work is described but started later
      - TODO fundamental: wait for I/O, timer, or concurrent completion
      - TODO compose asynchronous work with algorithms
   - TODO execution context
     - TODO thread pool: system_context
     - TODO GPU abstraction
   - TODO I/O context to deal with I/O and timers
   - TODO algorithms: when_all, when_any, bulk, and many more
   - TODO start asynchronous work
     - TODO synchronously (blocking the thread): sync_wait
     - TODO async_scope holding outstanding work and relasing resources once each item completes
   - TODO coroutine task
     - TODO co_await asynchronous work
     - TODO is itself asynchronous work, ie., a sender
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