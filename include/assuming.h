/// Copyright 2017-2020 Ghassan.pl
/// Usage of the works is permitted provided that this instrument is retained with
/// the works, so that any entity that uses the works is notified of this instrument.
/// DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
#pragma once

#include <fmt/format.h>
#include <gsl/gsl_assert>
#include <gsl/span>

#ifndef ASSUMING_DEBUG
#ifdef NDEBUG
#define ASSUMING_DEBUG 0
#else
#define ASSUMING_DEBUG 1
#endif
#endif

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define ASSUMING_SL {__LINE__, __FILE__, __PRETTY_FUNCTION__}

#if ASSUMING_DEBUG

#define Assuming(exp, ...) { if (decltype(auto) _assuming_exp_v = (exp); GSL_UNLIKELY(!_assuming_exp_v)) \
	ReportAssumptionFailure(ASSUMING_SL, #exp " will evalute to true", { { #exp, fmt::format("{}", _assuming_exp_v) } }, detail::AdditionalDataToString(__VA_ARGS__)); }

#define AssumingNull(exp, ...) { if (decltype(auto) _assuming_exp_v = (exp); GSL_UNLIKELY(_assuming_exp_v != nullptr)) \
	ReportAssumptionFailure(ASSUMING_SL, #exp " will be null", { { #exp, fmt::format("{}", fmt::ptr(_assuming_exp_v)) } }, detail::AdditionalDataToString(__VA_ARGS__)); }

#define AssumingNotNull(exp, ...) { if (decltype(auto) _assuming_exp_v = (exp); GSL_UNLIKELY(_assuming_exp_v == nullptr)) \
	ReportAssumptionFailure(ASSUMING_SL, #exp " will not be null", { { #exp, fmt::format("{}", fmt::ptr(_assuming_exp_v)) } }, detail::AdditionalDataToString(__VA_ARGS__)); }

#define AssumingBinOp(a, b, op, text, ...) { decltype(auto) _assuming_a_v = (a); decltype(auto) _assuming_b_v = (b); if (GSL_UNLIKELY(!(_assuming_a_v op _assuming_b_v))) \
	ReportAssumptionFailure(ASSUMING_SL, #a " will " text " " #b, { { #a, fmt::format("{}", _assuming_a_v) }, { #b, fmt::format("{}", _assuming_b_v) } }, detail::AdditionalDataToString(__VA_ARGS__)); }

#define AssumingEqual(a, b, ...) AssumingBinOp(a, b, ==, "be equal to", __VA_ARGS__)
#define AssumingNotEqual(a, b, ...) AssumingBinOp(a, b, !=, "not be equal to", __VA_ARGS__)
#define AssumingGreater(a, b, ...) AssumingBinOp(a, b, >, "be greater than", __VA_ARGS__)
#define AssumingLess(a, b, ...) AssumingBinOp(a, b, <, "be less than", __VA_ARGS__)
#define AssumingGreaterEqual(a, b, ...) AssumingBinOp(a, b, >=, "be greater or equal to", __VA_ARGS__)
#define AssumingLessEqual(a, b, ...) AssumingBinOp(a, b, <=, "be less or equal to", __VA_ARGS__)

#define AssumingEmpty(exp, ...) { using std::empty; using std::size; if (decltype(auto) _assuming_exp_v = (exp); GSL_UNLIKELY(!empty(_assuming_exp_v))) \
	ReportAssumptionFailure(ASSUMING_SL, #exp " will be empty", { { "size of " #exp, fmt::format("{}", size(_assuming_exp_v)) } }, detail::AdditionalDataToString(__VA_ARGS__)); }

#define AssumingNotEmpty(exp, ...) { using std::empty; using std::size; if (decltype(auto) _assuming_exp_v = (exp); GSL_UNLIKELY(empty(_assuming_exp_v))) \
	ReportAssumptionFailure(ASSUMING_SL, #exp " will not be empty", { { "size of " #exp, fmt::format("{}", size(_assuming_exp_v)) } }, detail::AdditionalDataToString(__VA_ARGS__)); }

#define AssumingNullOrEmpty(exp, ...) { using std::empty; using std::size; if (decltype(auto) _assuming_exp_v = (exp); GSL_UNLIKELY(!detail::IsNullOrEmpty(_assuming_exp_v))) \
	ReportAssumptionFailure(ASSUMING_SL, #exp " will be null or empty", { { #exp, _assuming_exp_v ? fmt::format("'{}'", _assuming_exp_v) : "(null)" } }, detail::AdditionalDataToString(__VA_ARGS__)); }

#define AssumingNotNullOrEmpty(exp, ...) { using std::empty; using std::size; if (decltype(auto) _assuming_exp_v = (exp); GSL_UNLIKELY(detail::IsNullOrEmpty(_assuming_exp_v))) \
	ReportAssumptionFailure(ASSUMING_SL, #exp " will not be null or empty", { { #exp, _assuming_exp_v ? fmt::format("'{}'", _assuming_exp_v) : "(null)" } }, detail::AdditionalDataToString(__VA_ARGS__)); }

#define AssumingValidIndex(_index, _container, ...) { using std::size; decltype(auto) _assuming_index = (_index); decltype(auto) _assuming_container = (_container); const auto _assuming_container_size = size(_assuming_container); \
	if (GSL_UNLIKELY(!(_assuming_index >= 0 && size_t(_assuming_index) < _assuming_container_size))) { \
		ReportAssumptionFailure(ASSUMING_SL, #_index " will be a valid index to " #_container, { \
			{ #_index, fmt::format("{}", _assuming_index) }, \
			{  "size of " #_container, fmt::format("{}", _assuming_container_size) }, \
		}, detail::AdditionalDataToString(__VA_ARGS__)); } }

#else

#define Assuming(exp, ...) GSL_ASSUME(!(exp))
#define AssumingNull(exp, ...) GSL_ASSUME(!((exp) != nullptr))
#define AssumingNotNull(exp, ...) GSL_ASSUME(!((exp) == nullptr))
#define AssumingBinOp(a, b, op, text, ...) GSL_ASSUME(((a) op (b)))
#define AssumingEqual(a, b, ...) AssumingBinOp(a, b, ==, "be equal to", __VA_ARGS__)
#define AssumingNotEqual(a, b, ...) AssumingBinOp(a, b, !=, "not be equal to", __VA_ARGS__)
#define AssumingGreater(a, b, ...) AssumingBinOp(a, b, >, "be greater than", __VA_ARGS__)
#define AssumingLess(a, b, ...) AssumingBinOp(a, b, <, "be less than", __VA_ARGS__)
#define AssumingGreaterEqual(a, b, ...) AssumingBinOp(a, b, >=, "be greater or equal to", __VA_ARGS__)
#define AssumingLessEqual(a, b, ...) AssumingBinOp(a, b, <=, "be less or equal to", __VA_ARGS__)
#define AssumingEmpty(exp, ...) { using std::empty; GSL_ASSUME(empty(exp)); }
#define AssumingNotEmpty(exp, ...) { using std::empty; using std::size; GSL_ASSUME(!empty(exp)); }
#define AssumingNullOrEmpty(exp, ...) { using std::empty; using std::size; GSL_ASSUME(detail::IsNullOrEmpty(exp));  }
#define AssumingNotNullOrEmpty(exp, ...) { using std::empty; using std::size; GSL_ASSUME(!detail::IsNullOrEmpty(exp)); }
#define AssumingValidIndex(_index, _container, ...) { using std::size; decltype(auto) _assuming_index = (_index); GSL_ASSUME(((_assuming_index) >= 0 && size_t(_assuming_index) < size(_container))); }

#endif

namespace detail
{
	inline bool IsNullOrEmpty(const char* str) { return str == nullptr || str[0] == 0; }
	template <typename T>
	inline bool IsNullOrEmpty(T&& str) { using std::empty; return empty(str); }

	inline std::string AdditionalDataToString() { return {}; }
	template <typename T, typename... ARGS>
	inline std::string AdditionalDataToString(T&& fmt, ARGS&&... args) {
		return fmt::format(std::forward<T>(fmt), std::forward<ARGS>(args)...);
	}

	struct source_location {
		long line;
		std::string_view file_name;
		std::string_view function_name;
	};
}

void ReportAssumptionFailure(detail::source_location where, std::string_view expectation, std::initializer_list<std::pair<std::string_view, std::string>> values, std::string data);

/*
#define AssumingBetween(v, a, b, ...) { Assuming_GET(v_v, v); Assuming_GET(a_v, a); Assuming_GET(b_v, b); \
	Assuming_IFFALSE(v_v >= a_v && v_v < b_v) { Assuming_START(); Assuming_FAILED(#v " will be between " #a " (inclusive) and " #b " (exclusive"); \
	Assuming_VAL(v_v, #v); Assuming_VAL(a_v, #a); Assuming_VAL(b_v, #b); Assuming_FINISH(__VA_ARGS__); } }
*/