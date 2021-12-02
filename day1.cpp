// Aoc - 2021 Day 1: Sonar Sweep
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "1"

#define FIRST_STAR  "1292"
#define SECOND_STAR "1262"

#ifdef THISDAY
#define TODAY DAY THISDAY "/"
#else
#define TODAY
#endif  // THISDAY

using namespace ranges;

namespace
{
auto Init(const string & input)
{
  return input | views::tokenize(regex("\n"), -1) | views::transform(sub_match_to_int()) |
         to<vector>;
}

struct Solve
{
  decltype(Init(string())) data;
  Solve(const string & inStr) { data = Init(inStr); };

  string Do()
  {
    return to_string(distance(data | views::adjacent_filter(
                                       [](auto x, auto y)
                                       {
                                         return x < y;
                                       })) -
                     1);
  }

  string Do2()
  {
    return to_string(count_if(views::zip(data, data | views::drop(3)),
                              [](auto x)
                              {
                                return x.first < x.second;
                              }));
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", "[x.]")
{
  REQUIRE(Solve(1 + R"(
199
200
208
210
200
207
240
269
260
263)")
            .Do() == "7");
}

TEST_CASE(TODAY "Part One", "[x.]")
{
#ifdef FIRST_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do() == FIRST_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do());
#endif  // FIRST_STAR
}

TEST_CASE(TODAY "Part Two", "[x.]")
{
#ifdef SECOND_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do2() == SECOND_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do2());
#endif
}
