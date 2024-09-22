<!--
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->
# std::execution Overview

## Terms

- completion signal
- environment

## Concepts

This section lists the concepts from `std::execution`.

<details>
<summary><code>operation_state&lt;<i>State</i>&gt;</code></summary>

Operation states represent asynchronous operations ready to be <code><a href=‘#start’>start</a></code>ed or executing. Operation state objects are normally neither movable nor copyable. Once <code><a href=‘#start’>start</a></code>ed the object needs to be kept alive until a <a href=‘#completion-signal’>completion signal</a> is received. Users don’t interact with operation states explicitly except when implementing new sender algorithms.

Required members for <code>_State_</code>:

- The type `operation_state_concept` is an alias for `operation_state_t` or a type derived thereof.
- <code><i>state</i>.<a href=‘#start’>start</a>() & noexcept</code>

<details>
<summary>Example</summary>

This example shows a simple operation state object which immediately completes successfully without any values (as <code><a href=‘#just’></a>()</code> would do). Normally <code><a href=‘#start’>start</a>()</code> initiates an asynchronous operation completing at some point later.

```c++
template <std::execution::receiver Receiver>
struct example_state
{
    using operation_state_concept = std::execution::operation_state_t;
    std::remove_cvref_t<Receiver> receiver;
    
    auto start() & noexcept {
        std::execution::set_value(std::move(this->receiver));
    }
};

static_assert(std::execution::operation_state<example_state<SomeReceiver>>);
```
</details> 
</details>

<details>
<summary><code>receiver&lt;<i>Receiver</i>&gt;</code></summary>

Receivers are used to receive <a href=‘#completion-signal’>completion signals</a>:
when an asynchronous operation completes the corresponding <a href=‘#completion-signal’>completion signal</a>
is called with the appropriate arguments. In addition receivers provide access to the
<a href=‘#environment’>environment</a> for the operation via the <a href=‘#get-env’><code>get_env</code></a> method.
Users don’t interact with receivers explicitly except when implementing new sender algorithms.

Required members for <code>_Receiver_</code>:

- The type `receiver_concept` is an alias for `receiver_t` or a type derived thereof`.
- Rvalues of type <code>_Receiver_</code> are movable.
- Lvalues of type <code>_Receiver_</code> are copyable.
- <code><a href=‘#get-env’>std::execution::get_env</a>(_receiver_)</code> returns an object. By default this operation returns <code><a href=‘empty-env’>std::execution::empty_env</a></code>.

Typical members for <code>_Receiver_</code>:

- <code><a href=‘get_env’>get_env</a>() const noexcept</code>
- <code><a href=‘set_value’>set_value</a>(args…) && noexcept -> void</code>
- <code><a href=‘set_error’>set_error</a>(error) && noexcept -> void</code>
- <code><a href=‘set_stopped’>set_stopped</a>() && noexcept -> void</code>

<details>
<summary>Example</summary>

The example receiver prints the name of each the received <a href=‘#completion-signal’>completion signal</a> before forwarding it to a receiver. It forwards the request for an environment (<code><a href=‘#get_env’>get_env</a><code>) to the nested receiver. This example is resembling a receiver as it would be used by a sender injecting logging of received signals.

```c++
template <std::execution::receiver NestedReceiver>
struct example_receiver
{
    using receiver_concept = std::execution::receiver_t;
    std::remove_cvref_t<NestedReceiver> nested;
    
    auto get_env() const noexcept {
        return std::execution::get_env(this->nested);
    }
    template <typename… A>
    auto set_value(A&&… a) && noexcept -> void {
        std::cout << “set_value\n”;
        std::execution::set_value(std::move(this->nested), std::forward<A>(a)…);
    }
    template <typename E>
    auto set_error(E&& e) && noexcept -> void {
        std::cout << “set_error\n”;
        std::execution::set_error(std::move(this->nested), std::forward<E>(e));
    }
    auto set_stopped() && noexcept -> void {
        std::cout << “set_stopped\n”;
        std::execution::set_stopped(std::move(this->nested));
    }
};

static_assert(std::execution::receiver<example_receiver<SomeReceiver>>);
```
</details> 
</details>

<details>
<summary><code>receiver_of&lt;<i>Receiver, Completions</i>&gt;</code></summary>

The concept <code>receiver_of&lt;<i>Receiver, Completions</i>&gt;</code> tests whether <code><a href=‘#receiver’>std::execution::receiver</a>&lt;_Receiver_&gt;</code> is true and if an object of type <code>_Receiver_</code> can be invoked with each of the <a href=‘#completion-signal’>completion signals</a> in <code>_Completions_</code>.

<details>
<summary>Example</summary>

The example defines a simple <code><a href=‘#receiver’>receiver</a><code> and tests whether it models `receiver_of` with different <a href=‘#completion-signal’>completion signals</a> in <code>_Completions_</code>
(note that not all cases are true).

```c++
struct example_receiver
{
    using receiver_concept = std::execution::receiver_t;
    
    auto set_value(int) && noexcept ->void {}
    auto set_stopped() && noexcept ->void {}
};


// matching the exact signals models receiver_of:
static_assert(std::execution::receiver_of<example_receiver,
    std::execution::completion_signals<
        std::execution::set_value_t(int),
        std::execution::set_stopped_t()
    >);
// providing a superset of signal models models receiver_of: 
static_assert(std::execution::receiver_of<example_receiver,
    std::execution::completion_signals<
        std::execution::set_value_t(int)
    >);
// providing only a subset of signals doesn’t model receiver_of:
static_assert(not std::execution::receiver_of<example_receiver,
    std::execution::completion_signals<
        std::execution::set_value_t(),
        std::execution::set_value_t(int)
    >);

```
</details>
</details>

<details>
<summary><code>scheduler&lt;<i>Scheduler</i>&gt;</code></summary>
Schedulers are used to specify the execution context where the asynchronous work is to be executed. A scheduler is a lightweight handle providing a <code><a href=‘#schedule’>schedule</a><code> operation yielding a <code><a href=‘sender’>sender<a/></code> with a value <a href=‘#completion-signal’>completion signal</a> without paramters. The completion is on the respective execution context.

Requirements for <code>_Scheduler_</code>:
- The type `scheduler_concept` is an alias for `scheduler_t` or a type derived thereof`.
- <code><a href=‘#schedule’>schedule</a>(_scheduler_) -> <a href=‘sender’>sender</a></code>
- The <a href=‘#get-completion-scheduler’>value completion scheduler</a> of the <code><a href=‘sender’>sender</a></code>’s <a href=‘#environment’>environment</a> is the <code>_scheduler_</code>:
    _scheduler_ == std::execution::get_completion_scheduler&lt;std::execution::set_value_t&gt;(
       std::execution::get_env(std::execution::schedule(_scheduler_))
    )
- <code>std::equality_comparable&lt;_Scheduler_&gt;</code>
- <code>std::copy_constructible&lt;_Scheduler_&gt;</code>
</details>

——-

- <code>sender&lt;<i>Sender</i>&gt;</code>
- <code>sender_in&lt;<i>Sender, Env</i> = std::execution::empty_env&gt;</code>
- <code>sender_to&lt;<i>Sender, Receiver</i>&gt;</code>
- <code>sends_stopped&lt;<i>Sender, Env</i> = std::execution::empty_env&gt;</code>

## Queries

- <code>forwarding_query(<i>query</i>) -> bool</code>
- <code>get_env(<i>queryable</i>)</code>
- <code>get_allocator(<i>env</i>)</code>
- <code>get_completion_scheduler(<i>env</i>)</code>
- <code>get_completion_signatures(<i>sender</i>, <i>env</i>)</code>
- <code>get_delegation_scheduler(<i>env</i>)</code>
- <code>get_domain(<i>env</i>)</code>
- <code>get_forward_progress_guarantee(<i>env</i>)</code>
- <code>get_scheduler(<i>env</i>)</code>
- <code>get_stop_token(<i>env</i>)</code>

### Customization Point Objects

- <code>connect(<i>sender, receiver</i>) -> <i>operation_state</i></code>
- <code>set_error(<i>error</i>) noexcept -> void</code>
- <code>set_stopped(<i>receiver</i>) noexcept -> void</code>
- <code>set_value(<i>receiver, value...</i>) noexcept -> void</code>
- <code>start(<i>state&amp;</i>) noexcept -> void</code>

## Senders

### Sender Factories

- <code>just(<i>value...</i>) -> <i>sender-of</i>&lt;set_value_t(<i>Value...</i>)&gt;</i></code>
- <code>just_error(<i>error</i>) -> <i>sender-of</i>&lt;set_error_t(<i>Error</i>)&gt;</i></code>
- <code>just_stopped() -> <i>sender-of</i>&lt;set_stopped_t()&gt;</code>
- <code>read_env(<i>query</i>) -> <i>sender-of</i>&lt;set_value_t(<i>query-result</i>)&gt;</code>
- <code>schedule(<i>scheduler</i>) -> <i>sender-of</i>&lt;set_value_t()&gt;</code>

### Sender Adaptors

- `bulk`
- `continues_on`
- `into_variant`
- `let_error`
- `let_stopped`
- `let_value`
- `on`
- `scheduler_from`
- `split`
- `starts_on`
- `stopped_as_error`
- `stopped_as_optional`
- `then`
- `upon_error()`
- `upon_stopped()`
- `when_all()`
- `when_all_with_variant()`
- `write_env()`

### Sender Consumers

- `sync_wait()`

## Helpers

- `as_awaitable`
- `with_awaitable_sender`
- `apply_sender`
- `completion_signatures`
- `completion_signatures_t`
- `connect_result_t`
- `default_domain`
- `empty_env`
- `env_of_t`
- `error_types_of_t`
- `fwd_env`
- `operation_state_t`
- `receiver_t`
- `run_loop`
- `scheduler_t`
- `schedule_result_t`
- `sender_adaptor_closure`
- `sender_t`
- `stop_token_of_t`
- `tag_of_t`
- `transform_sender`
- `transform_completion_signatures`
- `transform_completion_signatures_of`
- `value_types_of_t`

## Stop Tokens
