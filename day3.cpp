// Aoc - 2021 Day 2: Dive! ---
#include "stdafx.h"
#include "Utils.h"
#include <bitset>

//#define THISDAY "3"

#define FIRST_STAR  "2498354"
#define SECOND_STAR "3277956"

#ifdef THISDAY
#define TODAY DAY THISDAY "/"
#else
#define TODAY
#endif  // THISDAY

using namespace std::placeholders;  // for _1, _2, _3...

using namespace ranges;

namespace
{

using MyInt = std::bitset<12>;

auto Init(const string & input)
{
  return input | views::tokenize(regex("\n"), -1) | views::transform(sub_match_to_int<int, 2>()) |
         views::transform(
           [](auto i)
           {
             return MyInt(i);
           }) |
         to<vector>;
}
struct Solve
{
  decltype(Init(string())) data;

  Solve(const string & inStr) { data = Init(inStr); };

  static bool bitIsSet(int p, bool aDiffVal, MyInt i) { return i[p] != aDiffVal; }

  string Do()
  {
    auto gama = accumulate(views::iota(0, 12), MyInt(0),
                           [&](auto gama, auto p)
                           {
                             auto ones = count_if(data, std::bind_front(bitIsSet, p, 0));

                             return gama.set(p, ones > (double)data.size() / 2);
                           });

    return to_string(gama.to_ullong() * (~gama).to_ulong());
  }

  string Do2()
  {
    auto calc = [&](auto op)
    {
      return accumulate(views::iota(0, 12) | views::reverse, data,
                        [op](auto data, auto p)
                        {
                          if (data.size() == 1)
                            return data;

                          auto ones     = count_if(data, std::bind_front(bitIsSet, p, 0));
                          bool keepOnes = op(ones, (float)data.size() / 2);

                          return data | move |
                                 actions::remove_if(std::bind_front(bitIsSet, p, keepOnes));
                        });
    };
    auto oxygen = calc(greater_equal());
    auto co2    = calc(less());

    return to_string(oxygen.front().to_ulong() * co2.front().to_ulong());
  }
};
}  // namespace

#include "catch.hpp"

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
