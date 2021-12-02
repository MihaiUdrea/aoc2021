// Aoc - 2021 Day 2: Dive! ---
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "2"

#define FIRST_STAR  "1924923"
#define SECOND_STAR "1982495697"

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
             auto x = line | views::tokenize(regex(" "), -1) | to<vector>;

             return make_pair(x.front().str(), stoi(x.back()));
           }) |
         to<vector>;
}
struct Solve
{
  decltype(Init(string())) data;

  Solve(const string & inStr) { data = Init(inStr); };

  string Do()
  {
    auto pt = accumulate(data, Point(),
                         [](auto pt, auto i)
                         {
                           if (i.first == "forward")
                           {
                             pt.x += i.second;
                           }
                           else if (i.first == "down")
                           {
                             pt.y += i.second;
                           }
                           else if (i.first == "up")
                           {
                             pt.y -= i.second;
                           }

                           return pt;
                         });

    return to_string(pt.x * pt.y);
  }

  string Do2()
  {
    auto pt = accumulate(data, std::make_pair(Point(), 0),
                         [](auto a, auto i)
                         {
                           auto & [pt, aim] = a;

                           if (i.first == "forward")
                           {
                             pt.x += i.second;
                             pt.y += aim * i.second;
                           }
                           else if (i.first == "down")
                           {
                             aim += i.second;
                           }
                           else if (i.first == "up")
                           {
                             aim -= i.second;
                           }
                           return a;
                         });
    return to_string(pt.first.x * pt.first.y);
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", "[x.]")
{
  REQUIRE(Solve(1 + R"(
forward 5
down 5
forward 8
up 3
down 8
forward 2
)")
            .Do2() == "900");
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
