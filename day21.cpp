// Aoc - 2021 Day ?: ??????? ---
#include "stdafx.h"
#include "Utils.h"

//#define THISDAY "?"

#define FIRST_STAR "802452"
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
  return 1;
}
struct Solve
{
  decltype(Init(string())) data;

  Solve(const string & inStr) { data = Init(inStr); };

  int diePos{ 0 };
  using PosAndScore = pair<int, ULONGLONG>;
  map<pair<PosAndScore, PosAndScore>, pair<ULONGLONG, ULONGLONG>> hist;
  map<pair<PosAndScore, PosAndScore>, pair<ULONGLONG, ULONGLONG>> otherhist;

  string Do(int one, int two)
  {
    int sone = 0;
    int stwo = 0;
    while (true)  //(sone < 1000 && stwo < 1000)
    {
      for (auto a : views::indices(3))
        one += ++diePos;
      one = 1 + (one - 1) % 10;

      sone += one;

      if (sone >= 1000)
        return to_string(stwo * diePos);

      for (auto a : views::indices(3))
        two += ++diePos;
      two = 1 + (two - 1) % 10;

      stwo += two;
      if (stwo >= 1000)
        return to_string(sone * diePos);
    }

    return to_string(1);
  }

  map<PosAndScore, ULONGLONG> scoresHist;
  auto                        WinCounts(PosAndScore onePS, PosAndScore twoPS)
  {
    if (hist.contains(std::make_pair(onePS, twoPS)))
    {
      pair<ULONGLONG, ULONGLONG> res = hist[std::make_pair(onePS, twoPS)];

      if (0)
        cout << endl
             << std::setfill(' ') << std::setw(6) << "KNOW " << onePS.first << "," << onePS.second
             << "," << twoPS.first << "," << twoPS.second << "=" << res.first << "," << res.second;
      return res;
    }
    // else if (hist.contains(std::make_pair(twoPS, onePS)))
    //{
    //   pair<ULONGLONG, ULONGLONG> res = hist[std::make_pair(onePS, twoPS)];
    //
    //   return std::make_pair(res.second, res.first);
    // }
    else
    {
      pair<ULONGLONG, ULONGLONG> res;

      {
        for (auto a : views::ints(1, 4))
          for (auto b : views::ints(1, 4))
            for (auto c : views::ints(1, 4))
            {
              auto one  = onePS.first;
              auto sone = onePS.second;
              one += a + b + c;
              one = 1 + (one - 1) % 10;

              sone += one;

              if (sone >= 21)
                res.first++;
              else
              {
                // continue
                auto otherRes = WinCounts(twoPS, PosAndScore(one, sone));
                res.first += otherRes.second;
                res.second += otherRes.first;
              }
            }
      }

      if (scoresHist[onePS] < res.first)
        scoresHist[onePS] = res.first;

      hist[std::make_pair(onePS, twoPS)]      = res;
      otherhist[std::make_pair(twoPS, onePS)] = res;

      if (0)
        cout << endl
             << std::setfill(' ') << std::setw(6) << "NEW " << onePS.first << "," << onePS.second
             << "," << twoPS.first << "," << twoPS.second << "=" << res.first << "," << res.second;
      return res;
    }
  }

  string Do2(int one, int two)
  {
    auto res = WinCounts(PosAndScore(one, 0), PosAndScore(two, 0));
    return to_string(res.first > res.second ? res.first : res.second);
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", "[.]")
{
  REQUIRE(Solve(1 + R"(
first_input_line
)")
            .Do(4, 8) == "739785");
}

TEST_CASE(TODAY "Sample 2", "[x.]")
{
  REQUIRE(Solve(1 + R"(
first_input_line
)")
            .Do2(4, 8) == "444356092776315");
}

TEST_CASE(TODAY "Part Two", "[x.]")
{
#ifdef SECOND_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do2() == SECOND_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do2(7, 10));
#endif
}

TEST_CASE(TODAY "Part One", "[.]")
{
#ifdef FIRST_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do(7, 10) == FIRST_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do(7, 10));
#endif  // FIRST_STAR
}
