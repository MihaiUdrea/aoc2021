// Aoc - 2021 Day ?: ??????? ---
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "14"

#define FIRST_STAR  "3411"
#define SECOND_STAR "7477815755570"

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

  auto b = a.back() | views::tokenize(regex("\n"), -1) | views::transform(to_string_view()) |
           views::transform(
             [](auto line)
             {
               auto x = line | views::tokenize(regex("[A-Z]")) | to<vector<string>>;

               return std::make_pair(std::make_pair(x[0][0], x[1][0]), x[2][0]);
             }) |
           to<map>;
  std::list<char> xx = a[0] | views::all | to<std::list<char>>;
  return make_pair(xx, b);
}
struct Solve
{
  decltype(Init(string())) data;

  Solve(const string & inStr) { data = Init(inStr); };

  map<std::tuple<char, char, ULONGLONG>, map<char, ULONGLONG>> hist;

  auto Brute(string str, int depth)
  {
    data;
    for (auto step : views::ints | views::take(depth))
    { 
      auto rgn        = views::zip(str, str | views::drop(1));
      auto insertList = rgn | views::enumerate |
                        views::transform(
                          [&](auto pospair)
                          {
                            auto   pos = pospair.first;
                            string concat;
                            concat += pospair.second.first;
                            concat += pospair.second.second;

                            return std::make_pair(
                              pos, data.second[pospair.second]);  // data.second[pospair.second]);
                          }) |
                        to<map>;

      for (auto elem : insertList | views::reverse)
      {
        str.insert(str.begin() + elem.first + 1, elem.second);
      }
    }

    map<char, ULONGLONG> mymap;
    for (auto a : str)
      mymap[a]++;

    return make_pair(mymap, str);
  }

  map<char, ULONGLONG> Calc(char a, char b, ULONGLONG depth)
  {
    map<char, ULONGLONG> res;

    auto x = data.second[pair{ a, b }];

    if (depth == 1)
    {
      res[a]++;
      res[x]++;
      res[b]++;

      hist[std::make_tuple(a, b, depth)] = res;
      return res;
    }
    else
    {
      if (hist.contains(std::make_tuple(a, b, depth)))
        return hist[std::make_tuple(a, b, depth)];

      res        = Calc(a, x, depth - 1);
      auto right = Calc(x, b, depth - 1);

      for (auto a : right)
        res[a.first] += a.second;

      res[x]--;

      hist[std::make_tuple(a, b, depth)] = res;

      return res;
    }
  }

  string Do(int targetDepth = 10)
  {
    map<char, ULONGLONG> accMap;
    char                 prevChar = 0;

    auto rgn = views::zip(data.first, data.first | views::drop(1));
    for (auto el : rgn)
    {
      auto thisMap = Calc(el.first, el.second, targetDepth);
      for (auto a : thisMap)
        accMap[a.first] += a.second;

      if (prevChar)
        accMap[prevChar]--;

      prevChar = el.second;
    }

    auto minmax = minmax_element(accMap | views::values);

    return to_string(*minmax.max - *minmax.min);
  }

  string Do2() { return Do(40); }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", "[x.]")
{
  REQUIRE(Solve(1 + R"(
NNCB

CH -> B
HH -> N
CB -> H
NH -> C
HB -> C
HC -> B
HN -> C
NN -> C
BH -> H
NC -> B
NB -> B
BN -> B
BB -> N
BC -> B
CC -> N
CN -> C
)")
            .Do2() == "2188189693529");
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
