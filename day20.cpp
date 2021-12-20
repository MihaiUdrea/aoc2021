// day20 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "20"

#define FIRST_STAR  "5229"
#define SECOND_STAR "17009"

#ifdef THISDAY
#define TODAY            THISDAY "/"
#define HIDE_IF_OLD_TEST "[.]"
#else
#define TODAY
#define HIDE_IF_OLD_TEST
#endif  // THISDAY

// disable conversion warning
#pragma warning(disable : 4267)

using namespace ranges;

namespace
{
struct Solve
{
  /**/
  struct idAndrotation
  {
    int no;
    int rotation;

    auto operator<=>(const idAndrotation &) const = default;
  };

  map<int, CharMapLimits>        list;
  map<int, set<Point>>           bordersList;
  map<idAndrotation, set<Point>> allBordersList;
  map<idAndrotation, set<Point>> allArreasList;
  map<idAndrotation, set<Point>> allDragList;
  set<Point>                     data;
  set<int>                       imgEnhList;
  /**/

  vector<string> input;

  vector<Point> directions = { { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, -1 }, { 0, 0 },
                               { 0, 1 },   { 1, -1 }, { 1, 0 },  { 1, 1 } };
  Solve(const string & inStr)
  {
    using namespace views;

    CharMapLimits imgEnh;
    CharMapLimits image;

    for (auto chunk : inStr | views::tokenize(regex("\n\n"), -1) |
                        views::transform(to_string_view()) | views::take(1))
    {
      // int no;
      // for (auto line : chunk | views::tokenize(regex(":\n"), -1) |
      //                    views::transform(to_string_view()) | views::take(1))
      {
        string        s(chunk);
        CharMapLimits mapLimits(s);
        imgEnh = mapLimits;

        auto x = mapLimits.limit;
        cout << 1;
      }
    }
    for (auto chunk : inStr | views::tokenize(regex("\n\n"), -1) |
                        views::transform(to_string_view()) | views::drop(1) | views::take(1))

    {
      string        matrix(chunk);
      CharMapLimits mapLimits(matrix);
      // tileSize = (int)mapLimits.limit.y;
      image = mapLimits;
    }

    data = image.charMap['#'];

    for (auto p : imgEnh.charMap['#'])
      imgEnhList.insert(p.y);
  };

  string Do(int cnt = 2)
  {
    auto ss = to2Ds(
      data,
      [](auto & l, auto pos)
      {
        return Point(l.y, l.x);
      },
      [&](auto & l)
      {
        return string("#");
      },
      to2DsFlags::full_header, '.', 1);

    cout << endl << ss << endl;

    int  dx    = 180;
    auto limiX = minmax(data, less{}, &Point::x);
    auto limiY = minmax(data, less{}, &Point::y);
    for (auto i : views::indices(cnt))
    {
      set<Point> newData;
      for (auto [x, y] :
           views::cartesian_product(views::closed_indices(limiX.min.x - dx, limiX.max.x + dx),
                                    views::closed_indices(limiY.min.y - dx, limiY.max.y + dx)))
      {
        Point pt(x, y);
        auto  idx = accumulate(directions | views::transform(bind_back(std::plus(), pt)) |
                                 views::enumerate /* | views::reverse*/,
                               0,
                               [&](auto res, auto newPtAndPos)
                               {
                                auto [pos, newPt] = newPtAndPos;

                                auto & x = data;
                                if (data.contains(newPt))
                                  return res * 2 + 1;
                                else
                                  return res * 2;
                              });
        if (imgEnhList.contains(idx))
          newData.insert(pt);
      }

      if (1)
      {
        cout << "After " << cnt << endl;

        auto ss = to2Ds(
          newData,
          [](auto & l, auto pos)
          {
            return Point(l.y, l.x);
          },
          [&](auto & l)
          {
            return string("#");
          },
          to2DsFlags::full_header, '.', 1);
        cout << endl << ss << endl;
      }
      data = std::move(newData);
    }

    return to_string(data.size());
  }

  string Do2()
  {
    Do(50);

    Point topLeft(-58, -35);
    Point botRight(154, 164);

    return to_string(count_if(data,
                              [&](auto pt)
                              {
                                return pt.IsInsideRectangle(topLeft, botRight);
                              }));
  }
};
}  // namespace
#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", "[.]")
{
  REQUIRE(Solve(1 + R"(
..#.#..#####.#.#.#.###.##.....###.##.#..###.####..#####..#....#..#..##..###..######.###...####..#..#####..##..#.#####...##.#.#..#.##..#.#......#.###.######.###.####...#.##.##..#..#..#####.....#.#....###..#.##......#.....#..#..#..##..#...##.######.####.####.#.#...#.......#..#.#.#...####.##.#......#..#...##.#.##..#...##.#.##..###.#......#.#.......#.#.#.####.###.##...#.....####.#..#..#.##.#....##..#.####....##...##..#...#......#.#.......#.......##..####..#...#.#.#...##..#.#..###..#####........#..####......#..#

#..#.
#....
##..#
..#..
..###
)")
            .Do() == "35");
}

TEST_CASE(TODAY "Part Two", "[x.]")
{
#ifdef SECOND_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do2() == SECOND_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do2());
#endif
}

TEST_CASE(TODAY "Part One", "[.]")
{
#ifdef FIRST_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do() == FIRST_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do());
#endif  // FIRST_STAR
}
