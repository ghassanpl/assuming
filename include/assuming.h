/// Copyright 2017-2020 Ghassan.pl
/// Usage of the works is permitted provided that this instrument is retained with
/// the works, so that any entity that uses the works is notified of this instrument.
/// DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
#pragma once

#ifndef ASSUMING_DEBUG
#ifdef NDEBUG
#define ASSUMING_DEBUG 0
#else
#define ASSUMING_DEBUG 1
#endif
#endif

#ifndef ASSUMING_INCLUDE
#define ASSUMING_INCLUDE 1
#endif

#if ASSUMING_INCLUDE
#include <fmt/format.h>
#include <gsl/gsl_assert>
#include <magic_enum.hpp>
#endif

#if !ASSUMING_DEBUG
#ifndef GSL_ASSUME
#define GSL_ASSUME(...) ((void)(__VA_ARGS__))
#endif
#endif

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define ASSUMING_SL {__LINE__, __FILE__, __PRETTY_FUNCTION__, 0}

#if ASSUMING_DEBUG

#define Assume(exp, ...) { if (decltype(auto) _assuming_exp_v = (exp); GSL_UNLIKELY(!_assuming_exp_v)) \
	::ghassanpl::ReportAssumptionFailure(ASSUMING_SL, #exp " will evalute to true", { { #exp, fmt::format(FMT_STRING("{}"), ::ghassanpl::detail::GetFormattable(std::move(_assuming_exp_v))) } }, ::ghassanpl::detail::AdditionalDataToString(__VA_ARGS__)); }

#define AssumeNull(exp, ...) { if (decltype(auto) _assuming_exp_v = (exp); GSL_UNLIKELY(_assuming_exp_v != nullptr)) \
	::ghassanpl::ReportAssumptionFailure(ASSUMING_SL, #exp " will be null", { { #exp, fmt::format(FMT_STRING("{}"), fmt::ptr(_assuming_exp_v)) } }, ::ghassanpl::detail::AdditionalDataToString(__VA_ARGS__)); }

#define AssumeNotNull(exp, ...) { using ::std::to_address; if (decltype(auto) _assuming_exp_v = to_address(exp); GSL_UNLIKELY(_assuming_exp_v == nullptr)) \
	::ghassanpl::ReportAssumptionFailure(ASSUMING_SL, #exp " will not be null", { { #exp, fmt::format(FMT_STRING("{}"), fmt::ptr(_assuming_exp_v)) } }, ::ghassanpl::detail::AdditionalDataToString(__VA_ARGS__)); }

#define AssumeBinOp(a, b, op, text, ...) { decltype(auto) _assuming_a_v = (a); decltype(auto) _assuming_b_v = (b); if (GSL_UNLIKELY(!(_assuming_a_v op _assuming_b_v))) \
	::ghassanpl::ReportAssumptionFailure(ASSUMING_SL, #a " will " text " " #b, { \
		{ #a, fmt::format(FMT_STRING("{}"), ::ghassanpl::detail::GetFormattable(std::move(_assuming_a_v))) }, \
		{ #b, fmt::format(FMT_STRING("{}"), ::ghassanpl::detail::GetFormattable(std::move(_assuming_b_v))) } \
	}, ::ghassanpl::detail::AdditionalDataToString(__VA_ARGS__)); }

#define AssumeEqual(a, b, ...) AssumeBinOp(a, b, ==, "be equal to", __VA_ARGS__)
#define AssumeNotEqual(a, b, ...) AssumeBinOp(a, b, !=, "not be equal to", __VA_ARGS__)
#define AssumeGreater(a, b, ...) AssumeBinOp(a, b, >, "be greater than", __VA_ARGS__)
#define AssumeLess(a, b, ...) AssumeBinOp(a, b, <, "be less than", __VA_ARGS__)
#define AssumeGreaterEqual(a, b, ...) AssumeBinOp(a, b, >=, "be greater or equal to", __VA_ARGS__)
#define AssumeLessEqual(a, b, ...) AssumeBinOp(a, b, <=, "be less or equal to", __VA_ARGS__)

#define AssumeZero(a, ...) AssumeBinOp(a, 0, ==, "be equal to", __VA_ARGS__)

#define AssumeEmpty(exp, ...) { using std::empty; using std::size; if (decltype(auto) _assuming_exp_v = (exp); GSL_UNLIKELY(!empty(_assuming_exp_v))) \
	::ghassanpl::ReportAssumptionFailure(ASSUMING_SL, #exp " will be empty", { { "size of " #exp, fmt::format(FMT_STRING("{}"), size(_assuming_exp_v)) } }, ::ghassanpl::detail::AdditionalDataToString(__VA_ARGS__)); }

#define AssumeNotEmpty(exp, ...) { using std::empty; using std::size; if (decltype(auto) _assuming_exp_v = (exp); GSL_UNLIKELY(empty(_assuming_exp_v))) \
	::ghassanpl::ReportAssumptionFailure(ASSUMING_SL, #exp " will not be empty", { { "size of " #exp, fmt::format(FMT_STRING("{}"), size(_assuming_exp_v)) } }, ::ghassanpl::detail::AdditionalDataToString(__VA_ARGS__)); }

#define AssumeNullOrEmpty(exp, ...) { using std::empty; using std::size; if (decltype(auto) _assuming_exp_v = (exp); GSL_UNLIKELY(!::ghassanpl::detail::IsNullOrEmpty(_assuming_exp_v))) \
	::ghassanpl::ReportAssumptionFailure(ASSUMING_SL, #exp " will be null or empty", { { #exp, _assuming_exp_v ? fmt::format("'{}'", _assuming_exp_v) : "(null)" } }, ::ghassanpl::detail::AdditionalDataToString(__VA_ARGS__)); }

#define AssumeNotNullOrEmpty(exp, ...) { using std::empty; using std::size; if (decltype(auto) _assuming_exp_v = (exp); GSL_UNLIKELY(::ghassanpl::detail::IsNullOrEmpty(_assuming_exp_v))) \
	::ghassanpl::ReportAssumptionFailure(ASSUMING_SL, #exp " will not be null or empty", { { #exp, _assuming_exp_v ? fmt::format("'{}'", _assuming_exp_v) : "(null)" } }, ::ghassanpl::detail::AdditionalDataToString(__VA_ARGS__)); }

#define AssumeValidIndex(_index, _container, ...) { using std::size; decltype(auto) _assuming_index = (_index); decltype(auto) _assuming_container = (_container); const auto _assuming_container_size = size(_assuming_container); \
	if (GSL_UNLIKELY(!(_assuming_index >= 0 && size_t(_assuming_index) < _assuming_container_size))) { \
		::ghassanpl::ReportAssumptionFailure(ASSUMING_SL, #_index " will be a valid index to " #_container, { \
			{ #_index, fmt::format(FMT_STRING("{}"), _assuming_index) }, \
			{  "size of " #_container, fmt::format(FMT_STRING("{}"), _assuming_container_size) }, \
		}, ::ghassanpl::detail::AdditionalDataToString(__VA_ARGS__)); } }

#define Assert(exp, ...)																Assume(exp, __VA_ARGS__)
#define AssertNull(exp, ...)														AssumeNull(exp, __VA_ARGS__) 
#define AssertNotNull(exp, ...)													AssumeNotNull(exp, __VA_ARGS__)
#define AssertBinOp(a, b, op, text, ...) 								AssumeBinOp(a, b, op, text, __VA_ARGS__) 
#define AssertEqual(a, b, ...) 													AssumeEqual(a, b, __VA_ARGS__) 
#define AssertZero(a, ...) 															AssumeZero(a, __VA_ARGS__) 
#define AssertNotEqual(a, b, ...) 											AssumeNotEqual(a, b, __VA_ARGS__) 
#define AssertGreater(a, b, ...) 												AssumeGreater(a, b, __VA_ARGS__) 
#define AssertLess(a, b, ...) 													AssumeLess(a, b, __VA_ARGS__) 
#define AssertGreaterEqual(a, b, ...) 									AssumeGreaterEqual(a, b, __VA_ARGS__) 
#define AssertLessEqual(a, b, ...) 											AssumeLessEqual(a, b, __VA_ARGS__) 
#define AssertEmpty(exp, ...) 													AssumeEmpty(exp, __VA_ARGS__) 
#define AssertNotEmpty(exp, ...) 												AssumeNotEmpty(exp, __VA_ARGS__) 
#define AssertNullOrEmpty(exp, ...) 										AssumeNullOrEmpty(exp, __VA_ARGS__) 
#define AssertNotNullOrEmpty(exp, ...) 									AssumeNotNullOrEmpty(exp, __VA_ARGS__) 
#define AssertValidIndex(_index, _container, ...) 			AssumeValidIndex(_index, _container, __VA_ARGS__) 

#else

#define Assume(exp, ...) GSL_ASSUME(!!(exp))
#define AssumeNull(exp, ...) GSL_ASSUME(!((exp) != nullptr))
#define AssumeNotNull(exp, ...) GSL_ASSUME(!((exp) == nullptr))
#define AssumeBinOp(a, b, op, text, ...) GSL_ASSUME(((a) op (b)))
#define AssumeEqual(a, b, ...) AssumeBinOp(a, b, ==, "be equal to", __VA_ARGS__)
#define AssumeZero(a, ...) AssumeBinOp(a, 0, ==, "be equal to", __VA_ARGS__)
#define AssumeNotEqual(a, b, ...) AssumeBinOp(a, b, !=, "not be equal to", __VA_ARGS__)
#define AssumeGreater(a, b, ...) AssumeBinOp(a, b, >, "be greater than", __VA_ARGS__)
#define AssumeLess(a, b, ...) AssumeBinOp(a, b, <, "be less than", __VA_ARGS__)
#define AssumeGreaterEqual(a, b, ...) AssumeBinOp(a, b, >=, "be greater or equal to", __VA_ARGS__)
#define AssumeLessEqual(a, b, ...) AssumeBinOp(a, b, <=, "be less or equal to", __VA_ARGS__)
#define AssumeEmpty(exp, ...) { using std::empty; GSL_ASSUME(empty(exp)); }
#define AssumeNotEmpty(exp, ...) { using std::empty; using std::size; GSL_ASSUME(!empty(exp)); }
#define AssumeNullOrEmpty(exp, ...) { using std::empty; using std::size; GSL_ASSUME(::ghassanpl::detail::IsNullOrEmpty(exp));  }
#define AssumeNotNullOrEmpty(exp, ...) { using std::empty; using std::size; GSL_ASSUME(!::ghassanpl::detail::IsNullOrEmpty(exp)); }
#define AssumeValidIndex(_index, _container, ...) { using std::size; decltype(auto) _assuming_index = (_index); GSL_ASSUME(((_assuming_index) >= 0 && size_t(_assuming_index) < size(_container))); }

#define Assert(exp, ...)
#define AssertNull(exp, ...) 
#define AssertNotNull(exp, ...)
#define AssertBinOp(a, b, op, text, ...) 
#define AssertEqual(a, b, ...) 
#define AssertZero(a, ...) 
#define AssertNotEqual(a, b, ...) 
#define AssertGreater(a, b, ...) 
#define AssertLess(a, b, ...) 
#define AssertGreaterEqual(a, b, ...) 
#define AssertLessEqual(a, b, ...) 
#define AssertEmpty(exp, ...) 
#define AssertNotEmpty(exp, ...) 
#define AssertNullOrEmpty(exp, ...) 
#define AssertNotNullOrEmpty(exp, ...) 
#define AssertValidIndex(_index, _container, ...) 

#endif

namespace ghassanpl
{
	namespace detail
	{
		inline bool IsNullOrEmpty(const char* str) { return str == nullptr || str[0] == 0; }
		template <typename T>
		inline bool IsNullOrEmpty(T&& str) { using std::empty; return empty(str); }

#if ASSUMING_DEBUG
		inline std::string AdditionalDataToString() { return {}; }
		template <typename T, typename... ARGS>
		inline std::string AdditionalDataToString(T&& fmt, ARGS&&... args) {
			return fmt::format(std::forward<T>(fmt), std::forward<ARGS>(args)...);
		}

		template <typename T>
		auto GetFormattable(T&& val)
		{
			if constexpr (std::is_enum_v<T>)
				return magic_enum::enum_name(val);
			else if constexpr (std::is_pointer_v<T>)
				return fmt::ptr(std::forward<T>(val));
			else
				return std::forward<T>(val);
		}
#endif

		struct source_location {
			long line;
			std::string_view file_name;
			std::string_view function_name;
			long column;
		};
	}

	void ReportAssumptionFailure(::ghassanpl::detail::source_location where, std::string_view expectation, std::initializer_list<std::pair<std::string_view, std::string>> values, std::string data);
}
/*
#define AssumeBetween(v, a, b, ...) { Assume_GET(v_v, v); Assume_GET(a_v, a); Assume_GET(b_v, b); \
	Assume_IFFALSE(v_v >= a_v && v_v < b_v) { Assume_START(); Assume_FAILED(#v " will be between " #a " (inclusive) and " #b " (exclusive"); \
	Assume_VAL(v_v, #v); Assume_VAL(a_v, #a); Assume_VAL(b_v, #b); Assume_FINISH(__VA_ARGS__); } }
*/