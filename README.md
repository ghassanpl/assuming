# assuming
A small assertion library for C++17/20

### This is a work in progress

## About

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

![Assuming Example Handler](https://i.imgur.com/E49uu0h.png)

## Dependencies

* [fmt](https://github.com/fmtlib/fmt) (with possible upgrade path to `std::format`)
* [GSL](https://github.com/microsoft/GSL) (specifically `gsl::span` and `GSL_UNLIKELY` & `GSL_ASSUME`)
* [googletest](https://github.com/google/googletest) for testing only

## How to use

* Unless you specify `ASSUMING_DEBUG` to either `0` or `1`, the functionality is disabled with NDEBUG, like `assert()`
* Implement `ReportAssumptionFailure(...)`:
* Use the `Assuming*` macros

### Report Function

```c++
void ReportAssumptionFailure(detail::source_location where, std::string_view expectation, std::initializer_list<std::pair<std::string_view, std::string>> values, std::string data);
```

### Assuming* Macros

**Note:** All arguments will be evaluated only once.

#### Assuming(exp, [fmt, args...])
**Checked Expression**: `!exp`

#### AssumingNull(exp, [fmt, args...])
**Checked Expression**: `exp != nullptr`

#### AssumingNotNull(exp, [fmt, args...])
**Checked Expression**: `exp == nullptr`

#### AssumingBinOp(a, b, op, text, [fmt, args...])
**Checked Expression**: `a op b`

#### AssumingEqual(a, b, [fmt, args...])
**Checked Expression**: `a == b`

#### AssumingNotEqual(a, b, [fmt, args...])
**Checked Expression**: `a != b`

#### AssumingGreater(a, b, [fmt, args...])
**Checked Expression**: `a > b`

#### AssumingLess(a, b, [fmt, args...])
**Checked Expression**: `a < b`

#### AssumingGreaterEqual(a, b, [fmt, args...])
**Checked Expression**: `a >= b`

#### AssumingLessEqual(a, b, [fmt, args...])
**Checked Expression**: `a <= b`

#### AssumingEmpty(exp, [fmt, args...]) 
**Checked Expression**: `!empty(exp)` where `std::empty` is ADL-enabled
**Note**: `size(exp)` will also be evaluated (where `std::size` is ADL-enabled)

#### AssumingNotEmpty(exp, [fmt, args...]) 
**Checked Expression**: `empty(exp)` where `std::empty` is ADL-enabled
**Note**: `size(exp)` will also be evaluated (where `std::size` is ADL-enabled)

#### AssumingNullOrEmpty(exp, [fmt, args...])
**Checked Expression**: `!(exp == nullptr || exp[0] == 0;)` for `const char*`-decayed arguments, `!empty(exp)` with `std::empty` ADL-enabled otherwise
**Note**: `size(exp)` will also be evaluated (where `std::size` is ADL-enabled)

#### AssumingNotNullOrEmpty(exp, [fmt, args...])
**Checked Expression**: `(exp == nullptr || exp[0] == 0;)` for `const char*`-decayed arguments, `empty(exp)` with `std::empty` ADL-enabled otherwise
**Note**: `size(exp)` will also be evaluated (where `std::size` is ADL-enabled)

#### AssumingValidIndex(index, container, [fmt, args...])
**Checked Expression**: `!(index >= 0 && size_t(index) < size(container))` (where `std::size` is ADL-enabled)

## Upgrade path for C++20

* Change `fmt::format` to `std::format` (and do something about `fmt::ptr`)
* Use `std::source_location` instead of the macros neccessary right now

## TODO

* Add static_asserts for concepts the macro arguments must meet
* More and better tests
* Create a CMake project
* Maybe	put stuff in a namespace and add prefixes to the macros (ala gtest)
* Make sure the assumption macro does not evaluate its arguments