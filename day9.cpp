// Aoc - 2021 Day ?: ??????? ---
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "9"

#define FIRST_STAR  "489"
#define SECOND_STAR "1056330"

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
  // auto k =
  //   views::for_each(input | views::tokenize(regex("\n"), -1) | views::transform(to_string_view())
  //   |
  //                     views::enumerate,
  //                   [](auto a)
  //                   {
  //                     auto [line, s] = a;
  //                     return yield_from(s | views::enumerate |
  //                                       views::transform(
  //                                         [line](auto b)
  //                                         {
  //                                           auto [col, ch] = b;
  //                                           return std::make_pair(Point(line, col), ch - '0');
  //                                         }));
  //                   }) |
  //   to<map>;

  return input | views::tokenize(regex("\n"), -1) | views::transform(to_string_view()) |
         views::transform(StaticCast<string>()) | to<vector>;
}
struct Solve
{
  decltype(Init(string())) data;
  map<Point, int>          myMap;

  Solve(const string & inStr)
  {
    data = Init(inStr);
    for (auto [line, l] : data | views::enumerate)
    {
      for (auto [col, elem] : l | views::enumerate)
      {
        myMap[Point(line, col)] = elem - '0';
      }
    }
  };

  vector<Point> directions = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

  auto GetMins()
  {
    return myMap |
           views::filter(
             [&](auto pt)
             {
               return all_of(directions | views::transform(bind_back(std::plus(), pt.first)),
                             [&](auto newPt)
                             {
                               return !myMap.contains(newPt) || myMap[newPt] > myMap[pt.first];
                             });
             }) |
           to<vector>;
  }

  string Do()
  {
    auto mins = GetMins();

    return to_string(accumulate(mins | views::values, mins.size()));
  }

  auto Search(auto list, auto & done)
  {
    return accumulate(list, set<Point>{},
                      [&](auto next, auto pt)
                      {
                        done.insert(pt);

                        auto greaterNeigh =
                          directions | views::transform(bind_back(std::plus(), pt)) |
                          views::filter(
                            [&](auto newPt)
                            {
                              return !done.contains(newPt) && myMap.contains(newPt) &&
                                     myMap[newPt] >= myMap[pt] && myMap[newPt] != 9;
                            });

                        copy(greaterNeigh, inserter(next, next.begin()));
                        return next;
                      });
  }

  string Do2()
  {
    auto mins = GetMins();

    auto rgn = mins |
               views::transform(
                 [&](auto p)
                 {
                   auto start = p.first;

                   set<Point> done;

                   for (auto list = Search(set<Point>{ start }, done); !list.empty();
                        list      = Search(list, done))
                     ;

                   return done.size();
                 }) |
               to<vector>;

    sort(rgn);

    auto val = accumulate(rgn | views::reverse | views::take(3), (ULONGLONG)1, std::multiplies());
    return to_string(val);
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", "[x.]")
{
  REQUIRE(Solve(1 + R"(
2199943210
3987894921
9856789892
8767896789
9899965678
)")
            .Do2() == "1134");
}

TEST_CASE(TODAY "Part Two", "[x.]")
{
#ifdef SECOND_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do2() == SECOND_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do2());
#endif
}

TEST_CASE(TODAY "Part One", "[x.]")
{
#ifdef FIRST_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do() == FIRST_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do());
#endif  // FIRST_STAR
}
