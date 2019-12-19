# What's this?

Node.js allows to trap signals sent to it. And that mostly works, except with SIGABRT.

SIGABRT (abort) is correctly trapped by Node.js when sent to it from the outside (e.g. with the `kill` command), but not when the signal results from an `abort()` or a failed `assert()` function executed by a C or a C++ module (I don't know why :expressionless:).
And most (if not almost all) SIGABRT occurences come from the process itself, one of its modules, or libraries, or whatever run C or C++ code.

This small module is made specifically for these cases: it traps SIGABRT signals and prints a stacktrace starting from the signal-emitting function (NOT from the signal handler itself).

This module should be portable (tested on x86 with glibc and with musl), but on some environments, the stacktrace isn't available.

# How to use

Install: `npm i kuzzleio/abort-inspector`

Somewhere in a Node.js project (only needed once): `require('abort-inspector');`

*Note:* if you remove all listeners on the 'SIGABRT' event in Node.js, if this module is required _before_, then its trap will be removed. Require this module either after the removal of SIGABRT signals, or don't remove SIGABRT listeners.

# Stacktrace examples

musl (work in progress, in the meantime, I hate you musl):

```
=============== SIGABRT TRAPPED ===============
ip = 19089b5b0, sp = 7ffd1086e298, <unknown>
=== End of stack trace.
```

glibc:

```
================== SIGABRT TRAPPED ==================
ip = 7fdabbae042a, sp = 7ffe0ee05830, abort
ip = 7fdaa119c159, sp = 7ffe0ee05960, dumpProcess(Nan::FunctionCallbackInfo<v8::Value> const&)
ip = 7fdaa119c1d6, sp = 7ffe0ee05970, Nan::imp::FunctionCallbackWrapper(v8::FunctionCallbackInfo<v8::Value> const&)
ip = a5ff63, sp = 7ffe0ee059b0, v8::internal::FunctionCallbackArguments::Call(void (*)(v8::FunctionCallbackInfo<v8::Value> const&))
ip = ad710c, sp = 7ffe0ee05ab0, v8::internal::MaybeHandle<v8::internal::Object> v8::internal::(anonymous namespace)::HandleApiCallHelper<false>(v8::internal::Isolate*, v8::internal::Handle<v8::internal::HeapObject>, v8::internal::Handle<v8::internal::HeapObject>, v8::internal::Handle<v8::internal::FunctionTemplateInfo>, v8::internal::Handle<v8::internal::Object>, v8::internal::BuiltinArguments)
ip = ad7d5f, sp = 7ffe0ee05b90, v8::internal::Builtin_HandleApiCall(int, v8::internal::Object**, v8::internal::Isolate*)
ip = 2d76c1042fd, sp = 7ffe0ee05c00, <unknown>
ip = 2d76c1bd1d6, sp = 7ffe0ee05c10, <unknown>
ip = 2d76c1bd1d6, sp = 7ffe0ee05c20, <unknown>
ip = 2d76c1bd1d6, sp = 7ffe0ee05c30, <unknown>
ip = 2d76c1bd1d6, sp = 7ffe0ee05c40, <unknown>
ip = 2d76c1bd1d6, sp = 7ffe0ee05c50, <unknown>
ip = 2d76c107b6a, sp = 7ffe0ee05c60, <unknown>
ip = 2d76c1f97e0, sp = 7ffe0ee05c70, <unknown>
ip = 2d76c1bd1d6, sp = 7ffe0ee05c80, <unknown>
ip = 2d76c10535f, sp = 7ffe0ee05c90, <unknown>
ip = 2d76c1bd1d6, sp = 7ffe0ee05ca0, <unknown>
ip = 2d76c10535f, sp = 7ffe0ee05cb0, <unknown>
ip = 2d76c1bd1d6, sp = 7ffe0ee05cc0, <unknown>
ip = 2d76c1bd1d6, sp = 7ffe0ee05cd0, <unknown>
ip = 2d76c1bd1d6, sp = 7ffe0ee05ce0, <unknown>
ip = 2d76c1bd1d6, sp = 7ffe0ee05cf0, <unknown>
ip = 2d76c1bd1d6, sp = 7ffe0ee05d00, <unknown>
ip = 2d76c1bd1d6, sp = 7ffe0ee05d10, <unknown>
ip = 2d76c1bd1d6, sp = 7ffe0ee05d20, <unknown>
ip = 2d76c1bd1d6, sp = 7ffe0ee05d30, <unknown>
ip = 2d76c1bd1d6, sp = 7ffe0ee05d40, <unknown>
ip = 2d76c1bd1d6, sp = 7ffe0ee05d50, <unknown>
ip = 2d76c1bd1d6, sp = 7ffe0ee05d60, <unknown>
ip = 2d76c104239, sp = 7ffe0ee05d70, <unknown>
ip = 2d76c104101, sp = 7ffe0ee05d80, <unknown>
ip = d6f94a, sp = 7ffe0ee05d90, v8::internal::Execution::Call(v8::internal::Isolate*, v8::internal::Handle<v8::internal::Object>, v8::internal::Handle<v8::internal::Object>, int, v8::internal::Handle<v8::internal::Object>*)
ip = a428f3, sp = 7ffe0ee06670, v8::Function::Call(v8::Local<v8::Context>, v8::Local<v8::Value>, int, v8::Local<v8::Value>*)
ip = 8c7011, sp = 7ffe0ee06770, node::InternalMakeCallback(node::Environment*, v8::Local<v8::Object>, v8::Local<v8::Function>, int, v8::Local<v8::Value>*, node::async_context)
ip = 8c7343, sp = 7ffe0ee06820, node::MakeCallback(v8::Isolate*, v8::Local<v8::Object>, v8::Local<v8::String>, int, v8::Local<v8::Value>*, node::async_context)
ip = 8b2aef, sp = 7ffe0ee06880, node::Environment::CheckImmediate(uv_check_s*)
ip = 141a4ac, sp = 7ffe0ee068d0, uv__run_check
ip = 1412f8e, sp = 7ffe0ee06910, uv_run
ip = 8d443d, sp = 7ffe0ee06980, node::Start(uv_loop_s*, int, char const* const*, int, char const* const*)
ip = 8cc8fd, sp = 7ffe0ee07890, node::Start(int, char**)
ip = 7fdabbacc2e1, sp = 7ffe0ee07920, __libc_start_main
ip = 89b1b1, sp = 7ffe0ee079e0, _start
=== End of stack trace.
```
