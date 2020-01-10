# assuming
A small assertion library for C++17/20

### This is a work in progress

## About

TODO

## Dependencies

* [fmt](https://github.com/fmtlib/fmt) (with possible upgrade path to `std::format`)
* [GSL](https://github.com/microsoft/GSL) (specifically `gsl::span` and `GSL_UNLIKELY` & `GSL_ASSUME`)

## How to use

* Unless you specify `ASSUMING_DEBUG` to either `0` or `1`, the functionality is disabled with NDEBUG, like `assert()`
* Implement `ReportAssumptionFailure(...)`
* Use the `Assuming*` macros

## Upgrade path for C++20

* Change `fmt::format` to `std::format` (and do something about `fmt::ptr`)
* Use `std::source_location` instead of the macro bullcrap neccessary right now

## TODO

* Add static_asserts for concepts the macro arguments must meet
* More and better tests