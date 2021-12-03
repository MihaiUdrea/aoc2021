// Aoc - 2021 Day 3: Binary Diagnostic ---
#include "stdafx.h"
#include "Utils.h"
#include <bitset>

#define THISDAY "3"

#define FIRST_STAR  "2498354"
#define SECOND_STAR "3277956"

#ifdef THISDAY
#define TODAY DAY THISDAY "/"
#else
#define TODAY
#endif  // THISDAY

using namespace ranges;

namespace
{

using MyInt = std::bitset<12>;

auto Init(const string & input)
{
  return input | views::tokenize(regex("\n"), -1) | views::transform(sub_match_to_int<int, 2>()) |
         views::transform(StaticCast<MyInt>()) | to<vector>;
}
struct Solve
{
  decltype(Init(string())) data;

  int power;

  Solve(const string & inStr)
  {
    data = Init(inStr);

    power = (inStr | views::tokenize(regex("\n"), -1)).front().length();
  };

  static bool bitIsSet(int p, bool aDiffVal, MyInt i) { return i[p] != aDiffVal; }

  string Do()
  {
    auto gama = accumulate(views::iota(0, power), MyInt(0),
                           [&](auto gama, auto p)
                           {
                             auto ones = count_if(data, std::bind_front(bitIsSet, p, 0));

                             return gama.set(p, ones > (double)data.size() / 2);
                           });

    auto epsilon = (~gama).to_ulong() % (1 << power);

    return to_string(gama.to_ullong() * epsilon);
  }

  auto Calc(auto comp)
  {
    return accumulate(views::iota(0, power) | views::reverse, data,
                      [comp](auto data, auto p)
                      {
                        if (data.size() == 1)
                          return data;

                        auto ones     = count_if(data, std::bind_front(bitIsSet, p, 0));
                        bool keepOnes = comp(ones, (float)data.size() / 2);

                        return data | move |
                               actions::remove_if(std::bind_front(bitIsSet, p, keepOnes));
                      });
  }

  string Do2()
  {
    auto oxygen = Calc(greater_equal());
    auto co2    = Calc(less());

    return to_string(oxygen.front().to_ulong() * co2.front().to_ulong());
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", "[x.]")
{
  REQUIRE(Solve(1 + R"(
00100
11110
10110
10111
10101
01111
00111
11100
10000
11001
00010
01010
)")
            .Do() == "198");
}

TEST_CASE(TODAY "Sample 2", "[x.]")
{
  REQUIRE(Solve(1 + R"(
00100
11110
10110
10111
10101
01111
00111
11100
10000
11001
00010
01010
)")
            .Do2() == "230");
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
