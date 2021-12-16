// Aoc - 2021 Day ?: ??????? ---
#include "stdafx.h"
#include "Utils.h"
#include <bitset>

//#define THISDAY "?"

#define FIRST_STAR  "923"
#define SECOND_STAR "258888628940"

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
  // cout << input;
  auto r = input | views::take(input.size() - 1) |
           views::transform(
             [](auto ch)
             {
               std::bitset<4> b(stoi(string() + ch, nullptr, 16));

               return b.to_string();
             }) |
           to<vector>;
  return r | views::join | to<string>;
}
struct Solve
{
  decltype(Init(string())) data;

  Solve(const string & inStr) { data = Init(inStr); };

  map<int, vector<int>> childsPos;
  map<int, int>         versions;
  map<int, ULONGLONG>   abc;
  map<int, int>         ids;

  int SplitPacks(const string & s, int off = 0, int maxx = -1)
  {
    std::bitset<3> v(s | views::drop(off) | views::take(3) | to<string>);
    versions[off] = v.to_ulong();
    std::bitset<3> t(s | views::drop(off + 3) | views::take(3) | to<string>);
    int            ti = t.to_ulong();
    ids[off]          = ti;
    if (ti == 4)
    {
      ULONGLONG val = 0;
      int       pos = off + 3 + 3;

      bool cont = false;
      do
      {
        std::bitset<5> a(s | views::drop(pos) | views::take(5) | to<string>);
        pos += 5;
        cont = a[4];
        a[4] = 0;
        val  = val * 16 + a.to_ulong();
      } while (cont);

      abc[off] = val;
      return pos;
    }
    std::bitset<1> i(s | views::drop(off + 3 + 3) | views::take(1) | to<string>);
    size_t         childOff = off + 3 + 3 + 1;
    if (i.any())
    {
      std::bitset<11> l(s | views::drop(off + 3 + 3 + 1) | views::take(11) | to<string>);
      int             childCnt = l.to_ulong();
      childOff += 11;
      for (auto i : views::indices(childCnt))
      {
        childsPos[off].push_back(childOff);
        childOff = SplitPacks(s, childOff);
      }
    }
    else
    {
      std::bitset<15> l(s | views::drop(off + 3 + 3 + 1) | views::take(15) | to<string>);
      auto            childSize = l.to_ulong();
      childOff += 15;
      auto packEnd = childOff + childSize;
      for (; childOff < packEnd; childOff = SplitPacks(s, childOff))
      {
        childsPos[off].push_back(childOff);
      }
    }

    return childOff;
  }

  string Do()
  {
    if (0)
    {
      auto x = data | views::enumerate |
               views::filter(
                 [](auto x)
                 {
                   return x.second == '1';
                 }) |
               views::transform(
                 [](auto x)
                 {
                   return Point(x.first, 0);
                 }) |
               to<vector>;

      auto ss = to2Ds(
        x,
        [](auto & l, auto pos)
        {
          return l;
        },
        [&](auto & l)
        {
          return string("1");
        },
        to2DsFlags::depth_on | to2DsFlags::full_header, '0', 1);
      cout << ss;
    }

    SplitPacks(data);

    return to_string(accumulate(versions | views::values, 0ul));
  }

  ULONGLONG Calc(int off)
  {
    auto callCalc = [&](auto ch)
    {
      return Calc(ch);
    };

    auto id = ids[off];

    if (id == 0)
    {
      return accumulate(childsPos[off], 0ull, std::plus{}, callCalc);
    }
    else if (id == 1)
    {
      return accumulate(childsPos[off], 1ull, std::multiplies{}, callCalc);
    }
    else if (id == 2)
    {
      // min
      return min(childsPos[off] | views::transform(callCalc));
    }
    else if (id == 3)
    {
      // max
      return max(childsPos[off] | views::transform(callCalc));
    }
    else if (id == 5)
    {
      // >
      return Calc(childsPos[off][0]) > Calc(childsPos[off][1]);
    }
    else if (id == 6)
    {
      // <
      return Calc(childsPos[off][0]) < Calc(childsPos[off][1]);
    }
    else if (id == 7)
    {
      // ==
      return Calc(childsPos[off][0]) == Calc(childsPos[off][1]);
    }
    else  // if (id == 4)
    {
      return abc[off];
    }
  }

  string Do2()
  {
    SplitPacks(data);

    return to_string(Calc(0));
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Part Two", "[x.]")
{
#ifdef SECOND_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do2() == SECOND_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do2());
#endif
}

TEST_CASE(TODAY "Sample 0", "[.]")
{
  REQUIRE(Solve(1 + R"(
D2FE28
)")
            .Do() == "6");
}

TEST_CASE(TODAY "Sample 1", "[.]")
{
  REQUIRE(Solve(1 + R"(
8A004A801A8002F478
)")
            .Do() == "16");
}

TEST_CASE(TODAY "Sample 2", "[.]")
{
  REQUIRE(Solve(1 + R"(
620080001611562C8802118E34
)")
            .Do() == "12");
}

TEST_CASE(TODAY "Sample 3", "[.]")
{
  REQUIRE(Solve(1 + R"(
C0015000016115A2E0802F182340
)")
            .Do() == "23");
}

TEST_CASE(TODAY "Sample 4", "[.]")
{
  REQUIRE(Solve(1 + R"(
A0016C880162017C3686B18A3D4780
)")
            .Do() == "31");
}

TEST_CASE(TODAY "Sample P2 1", "[x.]")
{
  REQUIRE(Solve(1 + R"(
C200B40A82
)")
            .Do2() == "3");
}

TEST_CASE(TODAY "Sample P2 2", "[x.]")
{
  REQUIRE(Solve(1 + R"(
04005AC33890
)")
            .Do2() == "54");
}

TEST_CASE(TODAY "Sample P2 3", "[x.]")
{
  REQUIRE(Solve(1 + R"(
880086C3E88112
)")
            .Do2() == "7");
}

TEST_CASE(TODAY "Sample P2 4", "[x.]")
{
  REQUIRE(Solve(1 + R"(
CE00C43D881120
)")
            .Do2() == "9");
}

TEST_CASE(TODAY "Sample P2 5", "[x.]")
{
  REQUIRE(Solve(1 + R"(
D8005AC2A8F0
)")
            .Do2() == "1");
}

TEST_CASE(TODAY "Sample P2 6", "[x.]")
{
  REQUIRE(Solve(1 + R"(
F600BC2D8F
)")
            .Do2() == "0");
}

TEST_CASE(TODAY "Sample P2 7", "[x.]")
{
  REQUIRE(Solve(1 + R"(
9C005AC2F8F0
)")
            .Do2() == "0");
}

TEST_CASE(TODAY "Sample P2 8", "[x.]")
{
  REQUIRE(Solve(1 + R"(
9C0141080250320F1802104A08
)")
            .Do2() == "1");
}

TEST_CASE(TODAY "Part One", "[x.]")
{
#ifdef FIRST_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do() == FIRST_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do());
#endif  // FIRST_STAR
}
