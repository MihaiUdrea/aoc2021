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

  // static bool HasBitOn

  string Do()
  {
    auto powers = views::iota(0, 12);

    auto gama = accumulate(powers, MyInt(0),
                           [&](auto gama, auto p)
                           {
                             auto ones = count_if(data,
                                                  [p](auto i)
                                                  {
                                                    return (bool)i[p];
                                                  });

                             return gama.set(p, ones > (double)data.size() / 2);
                           });

    return to_string(gama.to_ullong() * (~gama).to_ulong());
  }

  string Do2(int range = 12)
  {
    auto half = (double)data.size() / 2 + 0.5;

    auto powers = views::iota(0, range) | views::reverse;

    auto backup = data;

    auto oxygen = accumulate(powers, data,
                             [](auto d, auto p)
                             {
                               auto ones = count_if(d,
                                                    [p](auto i)
                                                    {
                                                      return (bool)i[p];
                                                    });

                               bool bit = ones >= (float)d.size() / 2;

                               return d | move |
                                      actions::remove_if(
                                        [p, bit](auto i)
                                        {
                                          return (bool)i[p] != bit;
                                        });
                             });

    auto co2 = accumulate(powers, 0,
                          [&](auto gama, auto p)
                          {
                            auto & x = data;

                            if (x.size() == 1)
                              return 1;

                            auto ones = count_if(data,
                                                 [p](auto i)
                                                 {
                                                   return (bool)i[p];
                                                 });

                            bool bit = ones < (float)data.size() / 2;

                            // remove others
                            x.erase(remove_if(x,
                                              [p, bit](auto i)
                                              {
                                                return (bool)i[p] != bit;
                                              }),
                                    x.end());

                            return gama | (bit ? 1 : 0) << p;
                          });

    co2 = data.front().to_ulong();

    return to_string(oxygen.front().to_ulong() * co2);
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
            .Do2(5) == "230");
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
