// Aoc - 2021 Day ?: ??????? ---
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "5"

#define FIRST_STAR  "5167"
#define SECOND_STAR "17604"

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
  return input | views::tokenize(regex("\n"), -1) | views::transform(to_string_view()) |
         views::transform(
           [](auto line)
           {
             auto x = line | views::tokenize(regex("\\d+")) | views::transform(sub_match_to_int()) |
                      to<vector>;
             return std::make_pair(Point(x[0], x[1]), Point(x[2], x[3]));
           }) |
         to<vector>;
}
struct Solve
{
  decltype(Init(string())) data;

  Solve(const string & inStr) { data = Init(inStr); };

  string Do(bool aDiagonal = false)
  {
    auto sets =
      data |
      views::transform(
        [aDiagonal](auto segment)
        {
          vector<Point> r_com;
          auto          minmaxX = std::minmax(segment.first.x, segment.second.x);
          auto          minmaxY = std::minmax(segment.first.y, segment.second.y);

          vector<Point> aa;
          if (aDiagonal || (minmaxY.first == minmaxY.second || minmaxX.first == minmaxX.second))
          {
            auto cnt = minmaxX.second - minmaxX.first + 1;
            if (cnt == 1)
              cnt = minmaxY.second - minmaxY.first + 1;

            auto rngX = views::linear_distribute(minmaxX.first, minmaxX.second, cnt);
            auto rngY = views::linear_distribute(minmaxY.first, minmaxY.second, cnt) | to<vector>;

            Point startPoint(minmaxX.first, minmaxY.first);
            if (startPoint != segment.first && startPoint != segment.second)
              actions::reverse(rngY);
            aa = views::zip(rngX, rngY) |
                 views::transform(
                   [](auto a)
                   {
                     return Point(get<0>(a), get<1>(a));
                   }) |
                 to<vector>;
          }
          return aa;
        }) |
      to<vector>;

    map<Point, int> mem;
    for (auto pt : sets | views::join)
    {
      mem[pt]++;
    }

    auto res = count_if(mem | views::values,
                        [](auto e)
                        {
                          return e > 1;
                        });

    return to_string(res);
  }

  string Do2() { return Do(true); }
};

}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", "[x.]")
{
  REQUIRE(Solve(1 + R"(
0,9 -> 5,9
8,0 -> 0,8
9,4 -> 3,4
2,2 -> 2,1
7,0 -> 7,4
6,4 -> 2,0
0,9 -> 2,9
3,4 -> 1,4
0,0 -> 8,8
5,5 -> 8,2
)")
            .Do2() == "12");
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
