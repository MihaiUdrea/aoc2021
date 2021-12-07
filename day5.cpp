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
  auto b = input | views::tokenize(regex("\n"), -1) | views::transform(to_string_view()) |
           views::transform(
             [](auto line)
             {
               auto x = line | views::tokenize(regex("\\d+")) |
                        views::transform(sub_match_to_int()) | to<vector>;
               return std::make_pair(Point(x[0], x[1]), Point(x[2], x[3]));
             }) |
           to<vector>;

  return b;
}
struct Solve
{
  decltype(Init(string())) data;

  Solve(const string & inStr) { data = Init(inStr); };

  string Do()
  {
    auto sets = data |
                views::transform(
                  [](auto segment)
                  {
                    vector<Point> r_com;
                    auto          minmaxX = std::minmax(segment.first.x, segment.second.x);
                    auto          minmaxY = std::minmax(segment.first.y, segment.second.y);

                    if (minmaxY.first == minmaxY.second)
                    {
                      r_com = views::zip(views::closed_iota(minmaxX.first, minmaxX.second),
                                         views::repeat(minmaxY.first)) |
                              views::transform(
                                [](auto a)
                                {
                                  return Point(get<0>(a), get<1>(a));
                                }) |
                              to<vector>;
                    }
                    else if (minmaxX.first == minmaxX.second)
                    {
                      r_com = views::zip(views::repeat(minmaxX.first),
                                         views::closed_iota(minmaxY.first, minmaxY.second)) |
                              views::transform(
                                [](auto a)
                                {
                                  return Point(get<0>(a), get<1>(a));
                                }) |
                              to<vector>;
                    }
                    return r_com;
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

  string Do2()
  {
    auto sets = data |
                views::transform(
                  [](auto segment)
                  {
                    vector<Point> r_com;
                    auto          minmaxX = std::minmax(segment.first.x, segment.second.x);
                    auto          minmaxY = std::minmax(segment.first.y, segment.second.y);

                    if (minmaxY.first == minmaxY.second)
                    {
                      r_com = views::zip(views::closed_iota(minmaxX.first, minmaxX.second),
                                         views::repeat(minmaxY.first)) |
                              views::transform(
                                [](auto a)
                                {
                                  return Point(get<0>(a), get<1>(a));
                                }) |
                              to<vector>;
                    }
                    else if (minmaxX.first == minmaxX.second)
                    {
                      r_com = views::zip(views::repeat(minmaxX.first),
                                         views::closed_iota(minmaxY.first, minmaxY.second)) |
                              views::transform(
                                [](auto a)
                                {
                                  return Point(get<0>(a), get<1>(a));
                                }) |
                              to<vector>;
                    }
                    else
                    {
                      auto  countx     = minmaxX.second - minmaxX.first;
                      auto  county     = minmaxY.second - minmaxY.first;
                      Point startPoint = segment.first;
                      Point endPoint   = segment.second;

                      auto seg = endPoint - startPoint;
                      auto sep = Point(seg.x / countx, seg.y / countx);

                      r_com = views::closed_iota(0, countx) |
                              views::transform(
                                [sep, startPoint](auto a)
                                {
                                  return startPoint + Point(sep.x * a, sep.y * a);
                                }) |
                              to<vector>;
                    }
                    return r_com;
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
