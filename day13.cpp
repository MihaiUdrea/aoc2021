// Aoc - 2021 Day ?: ??????? ---
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "13"

#define FIRST_STAR  "818"
#define SECOND_STAR "LRGPRECB"

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
  auto a = input | views::tokenize(regex("\n\n"), -1) | views::transform(StaticCast<string>()) |
           to<vector>;

  auto b = a.front() | views::tokenize(regex("\n"), -1) | views::transform(to_string_view()) |
           views::transform(
             [](auto line)
             {
               auto x = line | views::tokenize(regex(","), -1) | to<vector>;

               return Point(stoi(x.front()), stoi(x.back()));
             }) |
           to<set>;
  auto c = a.back() | views::tokenize(regex("\n"), -1) | views::transform(to_string_view()) |
           views::transform(
             [](auto line)
             {
               auto x = line | views::tokenize(regex("[xy]|\\d+")) | to<vector>;
               return std::make_pair(x.front().str()[0], stoi(x.back()));
             }) |
           to<vector>;
  return std::make_pair(b, c);
}
struct Solve
{
  decltype(Init(string())) data;

  Solve(const string & inStr) { data = Init(inStr); };

  auto calc(auto rgn)
  {
    for (auto [fold, nr] : rgn)
    {
      // find the ones below the fold line
      auto p = data.first |
               views::filter(
                 [&](Point pt)
                 {
                   return (fold == 'y' ? pt.y : pt.x) > nr;
                 }) |
               to<set>;

      // eliminate the ones below the fold line
      data.first = views::set_difference(data.first, p) | to<set>;

      // add their mirror to the list
      copy(p | views::transform(
                 [&](Point pt)
                 {
                   pt.rotate_point(fold == 'y' ? Point(pt.x, nr) : Point(nr, pt.y), 180);
                   return pt;
                 }),
           inserter(data.first, data.first.begin()));
    }
  }
  string Do()
  {
    calc(data.second | views::take(1));
    return to_string(data.first.size());
  }

  string Do2()
  {
    calc(data.second);
    auto s2s = to2Ds(
      data.first,
      [](auto & l, auto pos)
      {
        return Point{ l.x, l.y };
      },
      [&](auto & l)
      {
        return string("#");
      });
    cout << s2s;
    return "LRGPRECB";
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", "[x.]")
{
  REQUIRE(Solve(1 + R"(
6,10
0,14
9,10
0,3
10,4
4,11
6,0
6,12
4,1
0,13
10,12
3,4
3,0
8,4
1,10
2,14
8,10
9,0

fold along y=7
fold along x=5
)")
            .Do2() == "LRGPRECB");
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
