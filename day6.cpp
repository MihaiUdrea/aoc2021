// Aoc - 2021 Day ?: ??????? ---
#include "stdafx.h"
#include "Utils.h"

//#define THISDAY "?"

#define FIRST_STAR  "363101"
#define SECOND_STAR "1644286074024"

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
  auto v = input | views::tokenize(regex(","), -1) | views::transform(sub_match_to_int());

  return accumulate(v, map<int, ULONGLONG>{},
                    [](auto map, auto i)
                    {
                      map[i]++;
                      return map | move;
                    });

  auto vv = v | to<vector> | actions::sort;

  auto ww = vv |
            views::chunk_by(
              [](auto a, auto b)
              {
                return a == b;
              }) |
            to<vector>;

  auto r_count = ww | views::transform(
                        [](auto && r)
                        {
                          return distance(r);
                        });

  auto r_unique = vv | views::unique;

  return views::zip(r_unique, r_count) | to<map<int, ULONGLONG>>;

  return views::iota(0, 7) |
         views::transform(
           [&v](auto i)
           {
             return std::make_pair(i, (ULONGLONG)count(v, i));
           }) |
         to<map>;
}
struct Solve
{
  decltype(Init(string())) data;

  Solve(const string & inStr) { data = Init(inStr); };

  string Do(int count = 80)
  {
    auto prevs = accumulate(views::ints(0, count), pair<ULONGLONG, ULONGLONG>{},
                            [&](auto prevs, auto day)
                            {
                              auto dayMod = day % 7;

                              auto hatching = data[dayMod];

                              data[dayMod] += prevs.first;

                              prevs.first  = prevs.second;
                              prevs.second = hatching;

                              return prevs;
                            });

    auto ct = accumulate(data | views::values, (ULONGLONG)0);

    return to_string(ct + prevs.first + prevs.second);
  }

  string Do2() { return Do(256); }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 11", "[x.]")
{
  REQUIRE(Solve(1 + R"(
3,4,3,1,2
)")
            .Do() == "5934");
}

TEST_CASE(TODAY "Sample 1", "[x.]")
{
  REQUIRE(Solve(1 + R"(
3,4,3,1,2
)")
            .Do2() == "26984457539");
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
