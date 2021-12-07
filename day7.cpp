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

  auto Calc()
  {
    data = data | move | actions::sort;

    auto max       = max_element(data);
    auto min       = min_element(data);
    auto complData = data |
                     views::transform(
                       [&](auto i)
                       {
                         return *max - i;
                       }) |
                     to<vector>;

    auto partialSum  = data | views::partial_sum | to<vector>;
    auto partialSum2 = actions::reverse(data | views::reverse | views::partial_sum | to<vector>);
    auto complPartialSum =
      actions::reverse(complData | views::reverse | views::partial_sum | to<vector>);

    auto rgn = views::zip(data, views::zip(partialSum, partialSum2)) | to<vector>;

    int minVal = 1000000000;
    int level  = -1;
    for (auto [pos, elem] : rgn | views::enumerate)
    {
      auto [val, areas]          = elem;
      auto [leftArea, rightArea] = areas;

      auto leftRectangle = val * (pos + 1);
      auto leftGap       = leftRectangle - leftArea;

      auto rightTargetRectangle = (data.size() - pos) * val;

      auto cost = leftGap + rightArea - rightTargetRectangle;

      if (cost < minVal)
      {
        minVal = cost;
        level  = val;
      }
    }

    return std::make_pair(minVal, level);
  }

  auto CalcCost(int aTarget)
  {
    auto cost = accumulate(data | views::transform(
                                    [&](auto el)
                                    {
                                      auto val = abs(el - aTarget);
                                      return val * (val + 1) / 2;
                                    }),
                           0);

    return cost;
  }

  string Do()
  {
    auto midleIt = data.begin() + data.size() / 2;
    std::nth_element(data.begin(), midleIt, data.end());

    auto cost = accumulate(data, 0,
                           [&](auto val, auto el)
                           {
                             return val + abs(el - *midleIt);
                           });
    // Calc().first

    return to_string(cost);
  }

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
