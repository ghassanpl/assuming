/// Copyright 2017-2020 Ghassan.pl
/// Usage of the works is permitted provided that this instrument is retained with
/// the works, so that any entity that uses the works is notified of this instrument.
/// DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.

#define FMT_USE_WINDOWS_H 0
#define FMT_HEADER_ONLY 1
#include "../include/assuming.h"
#include <gtest/gtest.h>

struct AssumingTest;
AssumingTest* current_test = nullptr;

using namespace ghassanpl;

using name_value_pair = std::pair<std::string_view, std::string>;

struct AssumingTest : public ::testing::Test
{
  bool assumption_failed = false;
  detail::source_location last_where;
  std::string last_expectation;
  std::vector<name_value_pair> last_values;
  std::string last_data;

  size_t evaluation_count = 0;

  template <typename T>
  auto single_eval_check(T&& v)
  {
    evaluation_count++;
    return std::forward<T>(v);
  }

  void ReportAssumptionFailure(detail::source_location where, std::string_view expectation, std::initializer_list<name_value_pair> values, std::string data)
  {
    assumption_failed = true;
    last_where = where;
    last_expectation = expectation;
    //fmt::print("Failed assumption that {}\n", expectation);
    last_values = { values.begin(), values.end() };
    last_data = std::move(data);
  }

  template <typename T, typename U>
  static inline bool compare(T const& a, U const& b)
  {
    return std::equal(std::begin(a), std::end(a), std::begin(b), std::end(b));
  }

  void SetUp() override
  {
    current_test = this;
  }

  void TearDown() override
  {
    current_test = nullptr;
  }
};

#define EXPECT_ASSUMPTION_FAILED(func_name, ...) func_name(__VA_ARGS__, "test({}, {})", 0, 5); EXPECT_TRUE(assumption_failed) << #func_name; assumption_failed = false; \
  EXPECT_EQ(last_where.line, __LINE__); EXPECT_EQ(last_where.file_name, __FILE__); EXPECT_EQ(last_where.function_name, __PRETTY_FUNCTION__); EXPECT_EQ("test(0, 5)", last_data);
#define EXPECT_ASSUMPTION_SUCCEEDED(func_name, ...) func_name(__VA_ARGS__); EXPECT_FALSE(assumption_failed) << #func_name; assumption_failed = false;

TEST_F(AssumingTest, Assuming_works)
{
  EXPECT_ASSUMPTION_SUCCEEDED(Assuming, true);

  const bool value = false;
  EXPECT_ASSUMPTION_FAILED(Assuming, value);

  name_value_pair const values[] = { {"value", "false"} };
  ASSERT_TRUE(compare(values, last_values));
}

TEST_F(AssumingTest, AssumingNotNull_works)
{
  EXPECT_ASSUMPTION_SUCCEEDED(AssumingNotNull, &::ReportAssumptionFailure);

  const decltype(&::ReportAssumptionFailure) value = nullptr;
  EXPECT_ASSUMPTION_FAILED(AssumingNotNull, value);

  name_value_pair const values[] = { {"value", "0x0"} };
  ASSERT_TRUE(compare(values, last_values));
}

TEST_F(AssumingTest, AssumingNull_works)
{
  const decltype(&::ReportAssumptionFailure) value = nullptr;
  EXPECT_ASSUMPTION_SUCCEEDED(AssumingNull, value);

  EXPECT_ASSUMPTION_FAILED(AssumingNull, &::ReportAssumptionFailure);

  name_value_pair const values[] = { {"&::ReportAssumptionFailure", fmt::format("{}", fmt::ptr(&::ReportAssumptionFailure)) } };
  ASSERT_TRUE(compare(values, last_values));
}

TEST_F(AssumingTest, AssumingEqual_works)
{
  std::pair<int, double> q = { 5, 6 };
  EXPECT_ASSUMPTION_FAILED(AssumingEqual, q.first, q.second);

  name_value_pair const values[] = { {"q.first", "5"}, {"q.second", "6"} };
  ASSERT_TRUE(compare(values, last_values));

  const double value = 0.4;
  EXPECT_ASSUMPTION_SUCCEEDED(AssumingEqual, value, 0.4);
}

TEST_F(AssumingTest, AssumingNotEqual_works)
{
  EXPECT_ASSUMPTION_SUCCEEDED(AssumingNotEqual, 5, 6);

  const double value = 0.4;
  EXPECT_ASSUMPTION_FAILED(AssumingNotEqual, value, 0.4);

  name_value_pair const values[] = { {"value", "0.4"}, {"0.4", "0.4"} };
  ASSERT_TRUE(compare(values, last_values));
}

#define EXPECT_EVAL_COUNT(count, ...) __VA_ARGS__; EXPECT_EQ(std::exchange(evaluation_count, 0), count);
TEST_F(AssumingTest, assumings_evaluate_arguments_only_once)
{
  EXPECT_EVAL_COUNT(1, Assuming(single_eval_check(false)));

  EXPECT_EVAL_COUNT(1, AssumingNotNull(single_eval_check(decltype(&::ReportAssumptionFailure)(nullptr))));
  EXPECT_EVAL_COUNT(1, AssumingNull(single_eval_check(&::ReportAssumptionFailure)));

  EXPECT_EVAL_COUNT(2, AssumingEqual(single_eval_check(1), single_eval_check(2)));
  EXPECT_EVAL_COUNT(2, AssumingNotEqual(single_eval_check(1), single_eval_check(1)));
  EXPECT_EVAL_COUNT(2, AssumingGreater(single_eval_check(1), single_eval_check(1)));
  EXPECT_EVAL_COUNT(2, AssumingLess(single_eval_check(1), single_eval_check(1)));
  EXPECT_EVAL_COUNT(2, AssumingGreaterEqual(single_eval_check(1), single_eval_check(2)));
  EXPECT_EVAL_COUNT(2, AssumingLessEqual(single_eval_check(1), single_eval_check(0)));

  EXPECT_EVAL_COUNT(1, AssumingEmpty(single_eval_check(std::string{ "hello" })));
  EXPECT_EVAL_COUNT(1, AssumingNotEmpty(single_eval_check(std::string{})));

  EXPECT_EVAL_COUNT(1, AssumingNullOrEmpty(single_eval_check("hello")));
  const char* null_string = nullptr;
  std::string empty_string{};
  EXPECT_EVAL_COUNT(1, AssumingNotNullOrEmpty(single_eval_check(null_string)));

  EXPECT_EVAL_COUNT(2, AssumingValidIndex(single_eval_check(1), single_eval_check(empty_string)));
}

/// TODO: Make sure everything works when ASSUMING_DEBUG is not defined

void ghassanpl::ReportAssumptionFailure(detail::source_location where, std::string_view expectation, std::initializer_list<name_value_pair> values, std::string data)
{
  current_test->ReportAssumptionFailure(where, expectation, std::move(values), std::move(data));
}
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
