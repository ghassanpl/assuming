# assuming
A small assertion library for C++17/20

### This is a work in progress

## About

TODO

## Features

TODO

## Example

```c++
AssumingValidIndex(index, columns, "dt: {}", Game->DeltaTime);
```

The above is an example of an assumption that will be checked in debug mode, specifically, whether `index` is a valid index for `columns`.
If not, the library will call `ReportAssumptionFailure()` with appropriate arguments to give you as much information as possible.

In release mode, all expressions will still be evaluated, and their results will still be evaluated for validity, but no branching path will
be emitted. Instead, platform-specific `assume` functionality will be used to hint to the compiler that the code proceeds with the assumptions stated,
so it can emit optimized code based on those assumptions.

Here's an example of how a handler I use can display the information given to the function:

![Assuming Example Handler](https://raw.githubusercontent.com/ghassanpl/assuming/master/doc/msgbox.png)

## Dependencies

* [fmt](https://github.com/fmtlib/fmt) (with possible upgrade path to `std::format`)
* [GSL](https://github.com/microsoft/GSL) (specifically `gsl::span` and `GSL_UNLIKELY` & `GSL_ASSUME`)
* [googletest](https://github.com/google/googletest) for testing only

## How to use

* Unless you specify `ASSUMING_DEBUG` to either `0` or `1`, the functionality is disabled with NDEBUG, like `assert()`
* Implement `ReportAssumptionFailure(...)`:
* Use the `Assuming*` macros

## Report Function

```c++
void ReportAssumptionFailure(detail::source_location where, std::string_view expectation, std::initializer_list<std::pair<std::string_view, std::string>> values, std::string data);
```

## `Assuming*` Macros

**Note:** All arguments will be evaluated only once.

### `Assuming(exp, [fmt, args...])`
**Checked Expression**: `!exp`  
**Expectation Message**: `<exp> will evaluate to true`

### `AssumingNull(exp, [fmt, args...])`
**Checked Expression**: `exp != nullptr`  
**Expectation Message**: `<exp> will be null`

### `AssumingNotNull(exp, [fmt, args...])`
**Checked Expression**: `exp == nullptr`  
**Expectation Message**: `<exp> will not be null`

### `AssumingBinOp(a, b, op, text, [fmt, args...])`
**Checked Expression**: `!(a op b)`  
**Expectation Message**: `<a> will <text> <b>`

### `AssumingEqual(a, b, [fmt, args...])`
**Checked Expression**: `!(a == b)`  
**Expectation Message**: `<a> will be equal to <b>`

### `AssumingNotEqual(a, b, [fmt, args...])`
**Checked Expression**: `!(a != b)`  
**Expectation Message**: `<a> will not be equal to <b>`

### `AssumingGreater(a, b, [fmt, args...])`
**Checked Expression**: `!(a > b)`  
**Expectation Message**: `<a> will be greater than <b>`

### `AssumingLess(a, b, [fmt, args...])`
**Checked Expression**: `!(a < b)`  
**Expectation Message**: `<a> will be less than <b>`

### `AssumingGreaterEqual(a, b, [fmt, args...])`
**Checked Expression**: `!(a >= b)`  
**Expectation Message**: `<a> will be greater or equal to <b>`

### `AssumingLessEqual(a, b, [fmt, args...])`
**Checked Expression**: `!(a <= b)`  
**Expectation Message**: `<a> will be less or equal to <b>`

### `AssumingEmpty(exp, [fmt, args...])`
**Checked Expression**: `!empty(exp)` where `std::empty` is ADL-enabled  
**Expectation Message**: `<exp> will be empty`  
**Note**: `size(exp)` will also be evaluated (where `std::size` is ADL-enabled)

### `AssumingNotEmpty(exp, [fmt, args...])`
**Checked Expression**: `empty(exp)` where `std::empty` is ADL-enabled  
**Expectation Message**: `<exp> will not be empty`  
**Note**: `size(exp)` will also be evaluated (where `std::size` is ADL-enabled)

### `AssumingNullOrEmpty(exp, [fmt, args...])`
**Checked Expression**: `!(exp == nullptr || exp[0] == 0)` for `const char*`-decayed arguments, `!empty(exp)` with `std::empty` ADL-enabled otherwise  
**Expectation Message**: `<exp> will be null or empty`  
**Note**: `size(exp)` will also be evaluated (where `std::size` is ADL-enabled)

### `AssumingNotNullOrEmpty(exp, [fmt, args...])`
**Checked Expression**: `(exp == nullptr || exp[0] == 0)` for `const char*`-decayed arguments, `empty(exp)` with `std::empty` ADL-enabled otherwise  
**Expectation Message**: `<exp> will not be null or empty`  
**Note**: `size(exp)` will also be evaluated (where `std::size` is ADL-enabled)

### `AssumingValidIndex(index, container, [fmt, args...])`
**Checked Expression**: `!(index >= 0 && size_t(index) < size(container))` (where `std::size` is ADL-enabled)  
**Expectation Message**: `<index> will be a valid index to <container>`

## Upgrade path for C++20

* Change `fmt::format` to `std::format` (and do something about `fmt::ptr`)
* Use `std::source_location` instead of the macros neccessary right now

## Implementation notes

* To facilitate evaluating the arguments only once, they are evaluated via `decltype(auto) arg_value = (arg);`, so make sure that operation is valid and not expensive, as it is ALWAYS evaluated
* In release mode, the compiler assumption is hinted using `GSL_ASSUME` which specifies that "It is unspecified whether or not cond is actually evaluated.", so that may be a problem right now
* The function name is gotten using `__PRETTY_FUNCTION__` or `__FUNCSIG__`
* The `unlikely` attribute is applied to the condition
* Values for the `data` argument are formatted using the `fmt` library, so make sure they are formattable using it
* `[fmt, args...]` is also passed straight to `fmt` to be converted to `additional_data`; giving these arguments is optional
* Container arguments (e.g. to `AssumingEmpty`) are not formatted for `data`, only their size is

## TODO

* Add static_asserts for concepts the macro arguments must meet
* More and better tests
* Create a CMake project
* Maybe	put stuff in a namespace and add prefixes to the macros (ala gtest)
* Make sure the assumption macro does not evaluate its arguments
