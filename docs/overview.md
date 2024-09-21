<!--
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->
# std::execution Overview

## Concepts

- <code>operation_state&lt;<i>State</i>&gt;</code>
- <code>receiver&lt;<i>Receiver</i>&gt;</code>
- <code>receiver_of&lt;<i>Receiver, Completions</i>&gt;</code>
- <code>scheduler&lt;<i>Scheduler</i>&gt;</code>
- <code>sender&lt;<i>Sender</i>&gt;</code>
- <code>sender_in&lt;<i>Sender, Env</i> = std::execution::empty_env&gt;</code>
- <code>sender_to&lt;<i>Sender, Receiver</i>&gt;</code>
- <code>sends_stopped&lt;<i>Sender, Env</i> = std::execution::empty_env&gt;</code>
- <code>sender&lt;<i>Sender</i>&gt;</code>

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
