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
  return input | views::tokenize(regex("\n"), -1) | views::transform(to_string_view()) |
         views::transform(
           [](auto line)
           {
             auto x = line | views::tokenize(regex(" "), -1) | to<vector<string>>;

             return x;
           }) |
         to<vector>;
}
struct Solve
{
  decltype(Init(string())) data;

  Solve(const string & inStr) { data = Init(inStr); };

  string Do()
  {
    auto x = data | views::tail;

    auto y = data | views::chunk(18) | to<vector>;

    // auto f = z.front().at(0);

    auto getConstant = [&](int step, int line)
    {
      auto z = y | views::drop(step) | to<vector>;
      auto k = z.front().at(line - 1);
      return stoi(k[2]);
    };

    auto c05  = getConstant(0, 5);
    auto c06  = getConstant(0, 6);
    auto c016 = getConstant(0, 16);
    auto c16  = getConstant(1, 16);

    vector<Point> ptList;
    for (auto i : views::indices(14))
    {
      cout << "Digit " << i << endl;

      for (auto line : { 5, 6, 16 })
      {
        ptList.push_back({ i, line });
        cout << "Line " << line << " = " << getConstant(i, line) << endl;
      }
    }

    auto s2s = to2Ds(
      ptList,
      [](auto & l, auto pos)
      {
        return l;
      },
      [&](auto & l)
      {
        return to_string(getConstant(l.x, l.y));
      },
      0, 32, 4);
    cout << s2s << endl;

    {
      LONGLONG x = 0, y = 0, z = 0;

      auto w = views::repeat_n(9, 14) | to<vector>;

      if (0)
      {
        auto c36  = getConstant(3, 6);
        auto c216 = getConstant(2, 16);
        w[3]      = w[5] - (getConstant(5, 6) + getConstant(3, 16));

        w[2] = w[3] - (c36 + c216);
        // w[5] = 3;  // 1, 2,3

        // w[5] = 1;

        // w[6] = 9;
      }

      if (1)
      {
        auto getDepValBack = [&](auto stepa, auto stepb)
        {
          return w[stepa] - getConstant(stepa, 6) - getConstant(stepb, 16);
        };
        auto getDepVal = [&](auto stepa, auto stepb)
        {
          return w[stepa] + getConstant(stepa, 16) + getConstant(stepb, 6);
        };
        auto val = getDepValBack(3, 2);
        w[2]     = val;  // 6

        // w[5] = getDepVal(5, 4);
        w[5]  = getDepVal(4, 5);
        w[1]  = getDepValBack(6, 1);
        w[10] = getDepVal(9, 10);
        w[8]  = getDepValBack(11, 8);
        w[7]  = getDepValBack(12, 7);
        w[13] = 9 - 4;
      }
      for (auto step : views::indices(14))
      {
        auto v = z % 26;

        cout << endl << "Start " << step << endl << "mod : " << v << endl;
        x = z % 26 + getConstant(step, 6);

        if (getConstant(step, 5) == 26)
        {
          cout << "reset turn "
               << " x = " << x << " x-w[step] = " << x - w[step] << endl;
          if (x != w[step])
          {
            cout << "BAD" << endl;
          }
        }
        else
        {
          cout << "add turn" << endl;
        }
        z = z / getConstant(step, 5);

        if (x != w[step])
        {
          z = z * 26 + w[step] + getConstant(step, 16);
        }

        cout << " Step " << step << " Z " << z << endl;
      }

      cout << "SOIL " << endl;

      for (auto i : w)
        cout << i;

      return w | views::all | to<string>;

      cout << 1;
    }

    return to_string(1);
  }

  string Do2()
  {
    auto x = data | views::tail;

    auto y = data | views::chunk(18) | to<vector>;

    // auto f = z.front().at(0);

    auto getConstant = [&](int step, int line)
    {
      auto z = y | views::drop(step) | to<vector>;
      auto k = z.front().at(line - 1);
      return stoi(k[2]);
    };

    auto c05  = getConstant(0, 5);
    auto c06  = getConstant(0, 6);
    auto c016 = getConstant(0, 16);
    auto c16  = getConstant(1, 16);

    vector<Point> ptList;
    for (auto i : views::indices(14))
    {
      cout << "Digit " << i << endl;

      for (auto line : { 5, 6, 16 })
      {
        ptList.push_back({ i, line });
        cout << "Line " << line << " = " << getConstant(i, line) << endl;
      }
    }

    auto s2s = to2Ds(
      ptList,
      [](auto & l, auto pos)
      {
        return l;
      },
      [&](auto & l)
      {
        return to_string(getConstant(l.x, l.y));
      },
      0, 32, 4);
    cout << s2s << endl;

    {
      LONGLONG x = 0, y = 0, z = 0;

      auto w = views::repeat_n(1, 14) | to<vector>;

      if (0)
      {
        auto c36  = getConstant(3, 6);
        auto c216 = getConstant(2, 16);
        w[3]      = w[5] - (getConstant(5, 6) + getConstant(3, 16));

        w[2] = w[3] - (c36 + c216);
        // w[5] = 3;  // 1, 2,3

        // w[5] = 1;

        // w[6] = 9;
      }

      if (1)
      {
        auto getDepValBack = [&](auto stepa, auto stepb)
        {
          return w[stepa] - getConstant(stepa, 6) - getConstant(stepb, 16);
        };
        auto getDepVal = [&](auto stepa, auto stepb)
        {
          return w[stepa] + getConstant(stepa, 16) + getConstant(stepb, 6);
        };
        // auto val = getDepValBack(3, 2);
        // w[2]     = val;  // 6
        //  w[0]     = 2;
        //   w[4]  = 8;
        // w[5]  = getDepVal(4, 5);
        // w[1]  = getDepValBack(6, 1);
        // w[10] = getDepVal(9, 10);
        // w[8]  = getDepValBack(11, 8);
        // w[7]  = getDepValBack(12, 7);
        // w[13] = getDepVal(0, 13);
        w[3] += 3;
        w[2] = getDepValBack(3, 2);
        // w[5] = getDepVal(4, 5);

        w[4] = getDepValBack(5, 4);
        w[6] += 8;
        w[9] = getDepValBack(10, 9);
        // w[10] = getDepValBack(8, 10);
        w[11] += 1;
        w[12] += 5;
        w[0] = getDepValBack(13, 0);
      }
      for (auto step : views::indices(14))
      {
        auto v = z % 26;

        cout << endl << "Start " << step << endl << "mod : " << v << endl;
        x = z % 26 + getConstant(step, 6);

        if (getConstant(step, 5) == 26)
        {
          cout << "reset turn "
               << " x = " << x << " x-w[step] = " << x - w[step] << endl;
          if (x != w[step])
          {
            cout << "BAD" << endl;
          }
        }
        else
        {
          cout << "add turn" << endl;
        }
        z = z / getConstant(step, 5);

        if (x != w[step])
        {
          z = z * 26 + w[step] + getConstant(step, 16);
        }

        cout << " Step " << step << " Z " << z << endl;
      }

      cout << "SOIL " << endl;

      for (auto i : w)
        cout << i;

      return w | views::all | to<string>;

      cout << 1;
    }

    return to_string(1);
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", "[.]")
{
  REQUIRE(Solve(1 + R"(
first_input_line
)")
            .Do() == "");
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
