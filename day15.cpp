// Aoc - 2021 Day ?: ??????? ---
#include "stdafx.h"
#include "Utils.h"
#include <boost/graph/adjacency_list.hpp>

#define THISDAY "15"

#define FIRST_STAR  "687"
#define SECOND_STAR "2957"

#ifdef THISDAY
#define TODAY DAY THISDAY "/"
#else
#define TODAY
#endif  // THISDAY

using namespace ranges;
// using namespace boost;

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
        myMap[Point(col, line)] = elem - '0';
      }
    }
  };

  vector<Point> directions = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

  set<Point>        hist;
  map<Point, int>   dist;
  map<Point, Point> fromWhere;
  set<Point>        next;

  string Do()
  {
    data;

    auto limi = MinMax(myMap,
                       [](auto e)
                       {
                         return e.first;
                       });

    // hist.insert(limi.first);
    next.insert(limi.first);
    dist[limi.first] = 0;
    hist.insert(limi.first);

    bool added = true;
    do
    {
      added       = false;
      Point minPt = min(next, less{},
                        [&](auto a)
                        {
                          return dist[a];
                        });
      next.erase(minPt);

      for (auto newPt : directions | views::transform(bind_back(std::plus(), minPt)))
      {
        if (!myMap.contains(newPt))
          continue;
        if (hist.contains(newPt))
        {
          continue;
        }
        hist.insert(newPt);

        next.insert(newPt);
        dist[newPt]      = dist[minPt] + myMap[newPt];
        fromWhere[newPt] = minPt;
        added            = true;

        if (newPt == limi.second)
        {
          if (0)
          {
            auto from = fromWhere[limi.second];
            do
            {
              cout << endl
                   << "X: " << from.x << " Y:" << from.y << " total:" << dist[from]
                   << "localcost:" << myMap[from];
              from = fromWhere[from];
            } while (from != limi.first);

            auto x = to2Ds(
              dist,
              [](auto & l, auto pos)
              {
                return l.first;
              },
              [&](auto & l)
              {
                return to_string(dist[l.first]);
              },
              to2DsFlags::full_header, '.', 3);
            cout << x;
          }
          return to_string(dist[newPt]);
        }
      }

    } while (true);

    return to_string(1);
  }

  string Do2()
  {
    auto limi = MinMax(myMap,
                       [](auto e)
                       {
                         return e.first;
                       });

    using T = decltype(myMap);
    T newMap;
    for (auto i : views::indices(5))
      for (auto j : views::indices(5))
      {
        Point dx(i * (limi.second.x + 1), j * (limi.second.y + 1));
        auto  rgn = myMap | views::transform(
                              [&](auto a)
                              {
                               return std::make_pair(a.first + dx, 1 + (a.second + i + j - 1) % 9);
                              });
        copy(rgn, inserter(newMap, newMap.begin()));
      }
    myMap = newMap;

    return Do();
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", "[x.]")
{
  REQUIRE(Solve(1 + R"(
1163751742
1381373672
2136511328
3694931569
7463417111
1319128137
1359912421
3125421639
1293138521
2311944581
)")
            .Do2() == "315");
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
