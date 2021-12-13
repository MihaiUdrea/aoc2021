// Aoc - 2021 Day ?: ??????? ---
#include "stdafx.h"
#include "Utils.h"

//#define THISDAY "?"

//#define FIRST_STAR  ""
//#define SECOND_STAR ""

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
  return input | views::tokenize(regex("\n"), -1) | to<vector<string>>;
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
        // if (elem < '9')
        myMap[Point(line, col)] = elem - '0';
      }
    }
  };

  vector<Point> directions = { { -1, 0 }, { 1, 0 }, { 0, -1 },  { 0, 1 },
                               { -1, 1 }, { 1, 1 }, { -1, -1 }, { 1, -1 } };

  void Flash(auto p, auto & list)
  {
    auto pt = p;

    if (contains(list, pt))
      return;

    myMap[pt] = 0;

    list.insert(pt);

    for_each(directions | views::transform(bind_back(std::plus(), pt)),
             [&](auto newPt)
             {
               if (!myMap.contains(newPt))
                 return;

               myMap[newPt]++;
               if (myMap[newPt] > 9)
                 Flash(newPt, list);
             });
  }

  string Do()
  {
    auto x = data | views::tail;

    int cnt = 0;
    for (auto i : views::indices(100))
    {
      for (auto & oct : myMap)
        oct.second++;

      auto flashing = myMap |
                      views::filter(
                        [](auto e)
                        {
                          return e.second > 9;
                        }) |
                      views::keys | to<vector>;

      set<Point> allFlashing;
      for (auto flaOct : flashing)
        Flash(flaOct, allFlashing);

      for (auto fl : allFlashing)
        myMap[fl] = 0;
      cnt += allFlashing.size();
    }

    return to_string(cnt);
  }

  string Do2()
  {
    for (auto i : views::indices)
    {
      for (auto & oct : myMap)
        oct.second++;

      auto flashing = myMap |
                      views::filter(
                        [](auto e)
                        {
                          return e.second > 9;
                        }) |
                      views::keys | to<vector>;

      set<Point> allFlashing;
      for (auto flaOct : flashing)
        Flash(flaOct, allFlashing);

      for (auto fl : allFlashing)
        myMap[fl] = 0;
      if (allFlashing.size() == 100)
        return to_string(i);
    }
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", "[x.]")
{
  REQUIRE(Solve(1 + R"(
5483143223
2745854711
5264556173
6141336146
6357385478
4167524645
2176841721
6882881134
4846848554
5283751526
)")
            .Do2() == "");
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
