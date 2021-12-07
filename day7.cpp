// Aoc - 2021 Day ?: ??????? ---
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "7"

#define FIRST_STAR  "357353"
#define SECOND_STAR "104822130"

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
  return input | views::tokenize(regex(","), -1) | views::transform(sub_match_to_int()) |
         to<vector>;
}
struct Solve
{
  decltype(Init(string())) data;

  Solve(const string & inStr) { data = Init(inStr); };

  auto CalcCost(int aTarget)
  {
    return accumulate(data | views::transform(
                               [aTarget](auto el)
                               {
                                 auto val = abs(el - aTarget);
                                 return val * (val + 1) / 2;
                               }),
                      0);
  }

  auto Calc()
  {
    data = data | move | actions::sort;

    auto partialSum     = data | views::partial_sum;
    auto partialSumBack = actions::reverse(data | views::reverse | views::partial_sum | to<vector>);

    auto rgn = views::zip(data, views::zip(partialSum, partialSumBack));

    auto val = rgn | views::enumerate |
               views::transform(
                 [len = data.size()](auto a)
                 {
                   auto [pos, elem] = a;

                   auto [val, areas]          = elem;
                   auto [leftArea, rightArea] = areas;

                   auto leftRectangle = val * (pos + 1);
                   auto leftGap       = leftRectangle - leftArea;

                   auto rightTargetRectangle = (len - pos) * val;

                   auto cost = leftGap + rightArea - rightTargetRectangle;
                   return cost;
                 });

    auto min = min_element(val);
    return std::make_pair(*min, 0);
  }

  auto Part1()
  {
    auto midleIt = data.begin() + data.size() / 2;
    std::nth_element(data.begin(), midleIt, data.end());

    return accumulate(data | views::transform(
                               [midleIt](auto el)
                               {
                                 return abs(el - *midleIt);
                               }),
                      0);
  }

  string Do() { return to_string(Calc().first); }

  string Do2()
  {
    auto start = Calc();

    auto prevlevel = start.second;
    auto cost      = CalcCost(prevlevel);
    auto prevCost  = 10000000;
    do
    {
      prevlevel++;
      prevCost = cost;
      cost     = CalcCost(prevlevel);

    } while (cost < prevCost);

    return to_string(prevCost);
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 01", "[x.]")
{
  REQUIRE(Solve(1 + R"(
16,1,2,0,4,2,7,1,2,14
)")
            .Do() == "37");
}

TEST_CASE(TODAY "Sample 1", "[x.]")
{
  REQUIRE(Solve(1 + R"(
16,1,2,0,4,2,7,1,2,14
)")
            .Do2() == "168");
}

TEST_CASE(TODAY "Part One", "[.]")
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
