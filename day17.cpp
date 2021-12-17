  // Aoc - 2021 Day ?: ??????? ---
  #include "stdafx.h"
  #include "Utils.h"

  //#define THISDAY "?"

  #define FIRST_STAR  "3655"
  #define SECOND_STAR "1447"

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
    return input | views::tokenize(regex("-?\\d+")) | views::transform(sub_match_to_int()) |
           to<vector>;
  }
  struct Solve
  {
    decltype(Init(string())) data;

    Solve(const string & inStr) { data = Init(inStr); };
    map<Point, vector<Point>> inzone;
    map<Point, vector<Point>> histMap;

    string Do(bool printAll = false)
    {
      auto  x = data | views::tail;
      Point topLeft(data[0], -data[3]);
      Point botRight(data[1], -data[2]);

      for (auto [x, y] : views::cartesian_product(views::iota(0, 250), views::iota(-100, 1000)))
      {
        Point speed(x, -y);
        auto  initialSpeed = speed;
        // launch
        Point start;

        vector<Point> hist;
        hist.push_back(start);
        bool reached = false;
        while (start.y < botRight.y)
        {
          start = start + speed;
          hist.push_back(start);

          if (speed.x > 0)
            speed.x--;
          speed.y++;

          if (start.IsInsideRectangle(topLeft, botRight))
          {
            inzone[initialSpeed].push_back(start);
            reached = true;
          }
        }

        if (reached)
          histMap[initialSpeed] = std::move(hist);
      }

      auto el = min_element(inzone | views::keys);
      auto a  = *el;

      if (!printAll)
      {
        auto ss = to2Ds(
          histMap[a],
          [](auto & l, auto pos)
          {
            return l;
          },
          [&](auto & l)
          {
            return l.IsInsideRectangle(topLeft, botRight) ? string("0") : string("#");
          },
          to2DsFlags::full_header, ' ', 1);
        cout << endl << ss;
      }
      else
      {
        auto x = histMap | views::all | views::enumerate |
                 views::transform(
                   [](auto idxKeyVal)
                   {
                     auto [idx, keyVal] = idxKeyVal;

                     auto vec = keyVal.second | views::transform(
                                                  [idx](auto pt)
                                                  {
                                                    pt.z = idx;
                                                    return pt;
                                                  });
                     return vec;
                   }) |
                 views::join | to<vector>;
        auto ss = to2Ds(
          x,
          [](auto & l, auto pos)
          {
            return l;
          },
          [&](auto & l)
          {
            return l.IsInsideRectangle(topLeft, botRight) ? string("0") : string("#");
          },
          to2DsFlags::depth_on | to2DsFlags::full_header, ' ', 1);
        cout << endl << ss;
      }

      return to_string((-a.y) * (-a.y + 1) / 2);
    }

    string Do2()
    {
      Do(true);

      return to_string(inzone.size());
    }
  };
  }  // namespace

  #include "catch.hpp"

  TEST_CASE(TODAY "Sample 1", "[x.]")
  {
    REQUIRE(Solve(1 + R"(
  target area: x=20..30, y=-10..-5
  )")
              .Do() == "45");
  }
  TEST_CASE(TODAY "Sample 2", "[x.]")
  {
    REQUIRE(Solve(1 + R"(
  target area: x=20..30, y=-10..-5
  )")
              .Do2() == "112");
  }

  TEST_CASE(TODAY "Part Two", "[.]")
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
