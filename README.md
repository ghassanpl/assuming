# assuming
A small assertion library for C++17/20

### This is a work in progress

## About

TODO

# Example

```c++
int columns[] = {10, 20};
AssumingValidIndex(index, columns, "dt: {}", Game->DeltaTime);
```

The above is an example of an assumption that will be checked in debug mode, specifically, whether `index` is a valid index for `columns`.
If not, the library will call `ReportAssumptionFailure()` with appropriate arguments to give you as much information as possible.

Here's an example of how a handler I use can display the information given to the function:

![Assuming Example Handler](https://i.imgur.com/E49uu0h.png)

## Dependencies

* [fmt](https://github.com/fmtlib/fmt) (with possible upgrade path to `std::format`)
* [GSL](https://github.com/microsoft/GSL) (specifically `gsl::span` and `GSL_UNLIKELY` & `GSL_ASSUME`)

## How to use

* Unless you specify `ASSUMING_DEBUG` to either `0` or `1`, the functionality is disabled with NDEBUG, like `assert()`
* Implement `ReportAssumptionFailure(...)`:
   **Signature:**
  `void ReportAssumptionFailure(detail::source_location where, std::string_view expectation, std::initializer_list<std::pair<std::string_view, std::string>> values, std::string data);`
* Use the `Assuming*` macros

## Upgrade path for C++20

* Change `fmt::format` to `std::format` (and do something about `fmt::ptr`)
* Use `std::source_location` instead of the macro bullcrap neccessary right now

## TODO

* Add static_asserts for concepts the macro arguments must meet
* More and better tests
* Create a CMake project 