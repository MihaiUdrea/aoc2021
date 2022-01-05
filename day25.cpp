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
  CharMapLimits mapLimits(input);
  return mapLimits;
}
struct Solve
{
  decltype(Init(string())) data;

  Solve(const string & inStr) { data = Init(inStr); };

  string Do()
  {
    data;

    map<char, vector<pair<Point, Point>>> moveList;

    auto getstring = [](auto newPt)
    {
      set<Point> x = views::set_union(newPt.charMap['>'], newPt.charMap['v']) | to<set>;

      auto ss = to2Ds(
        x,
        [](auto & l, auto pos)
        {
          return Point(l.y, l.x);
        },
        [&](auto & l)
        {
          if (newPt.charMap['v'].contains(l))
            return string() + 'v';
          if (newPt.charMap['>'].contains(l))
            return string() + '>';
          return string() + '?';
        },
        true ? to2DsFlags::no_header : to2DsFlags::full_header, '.', 1);
      return ss;
    };

    int step = 0;
    do
    {
      if (0)
      {
        auto p1 = getstring(data);
        cout << "Step: " << step << endl;
        cout << p1 << endl;
      }

      step++;
      moveList.clear();
      for (auto pt : data.charMap['>'])
      {
        Point newPt = pt + Point{ 0, 1 };
        {
          newPt.x = newPt.x % data.limit.x;
          newPt.y = newPt.y % data.limit.y;

          if (data.charMap['>'].contains(newPt) || data.charMap['v'].contains(newPt))
          {
            // busy
          }
          else
          {
            // free
            moveList['>'].emplace_back(newPt, pt);
          }
        }
      }

      for (auto [newpt, oldPt] : moveList['>'])
      {
        data.charMap['>'].insert(newpt);
        data.charMap['>'].erase(oldPt);
      }

      {
        for (auto pt : data.charMap['v'])
        {
          Point newPt = pt + Point{ 1, 0 };
          {
            newPt.x = newPt.x % data.limit.x;
            newPt.y = newPt.y % data.limit.y;

            if (data.charMap['>'].contains(newPt) || data.charMap['v'].contains(newPt))
            {
              // busy
            }
            else
            {
              // free
              moveList['v'].emplace_back(newPt, pt);
            }
          }
        }

        for (auto [newpt, oldPt] : moveList['v'])
        {
          data.charMap['v'].insert(newpt);
          data.charMap['v'].erase(oldPt);
        }
      }

    } while (!moveList['v'].empty() || !moveList['>'].empty());
    return to_string(step);
  }

  string Do2()
  {
    data;
    return to_string(1);
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", "[x.]")
{
  REQUIRE(Solve(1 + R"(
v...>>.vv>
.vv>>.vv..
>>.>v>...v
>>v>>.>.v.
v>v.vv.v..
>.>>..v...
.vv..>.>v.
v.v..>>v.v
....v..v.>
)")
            .Do() == "58");
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
