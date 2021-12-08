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
             auto x = line | views::tokenize(regex("\\|"), -1) |
                      views::transform(to_string_view()) | to<vector>;

             auto a = x[0] | views::tokenize(regex("\\w+")) | views::transform(to_string_view()) |
                      to<vector>;
             auto b = x[1] | views::tokenize(regex("\\w+")) | views::transform(to_string_view()) |
                      to<vector>;
             return std::make_pair(a, b);
           }) |
         to<vector>;
}
struct Solve
{
  decltype(Init(string())) data;

  Solve(const string & inStr) { data = Init(inStr); };

  string Do()
  {
    map<int, int> digReq;
    digReq[1] = 2;
    digReq[4] = 4;
    digReq[7] = 3;
    digReq[8] = 7;

    auto req = digReq | views::values | to<set>;

    auto x = count_if(data | views::values | views::join,
                      [req](auto i)
                      {
                        return contains(req, i.length());
                      });

    return to_string(x);
  }

  string Do2()
  {
    map<int, int> digReq;
    digReq[1] = 2;
    digReq[4] = 4;
    digReq[7] = 3;
    digReq[8] = 7;

    auto revDigReq = digReq |
                     views::transform(
                       [](auto e)
                       {
                         return std::make_pair(e.second, e.first);
                       }) |
                     to<map>;

    auto req = digReq | views::values | to<set>;

    int sum = 0;
    for (auto a0 : data)
    {
      auto [a, b] = a0;
      map<int, string> digStr;
      for (auto s : a)
      {
        if (contains(req, s.length()))
        {
          digStr[revDigReq[s.length()]] = s;
        }
      }

      // decode between 1 and 4
      auto one   = digStr[1] | to<set>;
      auto four  = digStr[4] | to<set>;
      auto seven = digStr[7] | to<set>;
      auto eight = digStr[8] | to<set>;
      auto top   = views::set_difference(seven, one) | to<vector>;

      auto rgn6 = a |
                  views::filter(
                    [](auto i)
                    {
                      return i.length() == 6;
                    }) |
                  to<vector>;

      auto rgn5 = a |
                  views::filter(
                    [](auto i)
                    {
                      return i.length() == 5;
                    }) |
                  to<vector>;

      for (auto s : rgn6)
      {
        auto asset = s | views::all | to<set>;
        auto diff  = views::set_difference(one, asset) | to<vector>;
        if (diff.size() != 0)
          digStr[6] = s;
      }

      for (auto s : rgn6)
      {
        if (s == digStr[6])
          continue;

        auto asset = s | views::all | to<set>;
        auto diff  = views::set_difference(four, asset) | to<vector>;
        if (diff.size() != 0)
          digStr[0] = s;
        else
          digStr[9] = s;
      }

      for (auto s : rgn5)
      {
        auto asset = s | views::all | to<set>;
        auto diff  = views::set_difference(one, asset) | to<vector>;
        if (diff.size() == 0)
          digStr[3] = s;
      }

      auto prev = digStr | views::values | to<set>;

      auto six = digStr[6] | to<set>;

      for (auto s : rgn5)
      {
        if (contains(prev, s))
          continue;

        auto asset = s | views::all | to<set>;
        auto diff  = views::set_difference(six, asset) | to<vector>;
        if (diff.size() == 1)
          digStr[5] = s;
        else if (diff.size() == 2)
          digStr[2] = s;
      }

      auto revDigStr = digStr |
                       views::transform(
                         [](auto e)
                         {
                           sort(e.second);
                           return std::make_pair(e.second, e.first);
                         }) |
                       to<map<string, int>>;

      int nr = 0;
      for (auto s : b)
      {
        string s2(s);
        sort(s2);
        nr = nr * 10 + revDigStr[s2];
      }
      sum += nr;
    }

    return to_string(sum);

    auto rgn = data | views::values | views::join |
               views::filter(
                 [req](auto i)
                 {
                   return contains(req, i.length());
                 }) |
               to<vector>;

    auto x = distance(rgn);

    return to_string(x);
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 0", "[x.]")
{
  REQUIRE(Solve(1 + R"(
acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab | cdfeb fcadb cdfeb cdbaf
)")
            .Do2() == "5353");
}

TEST_CASE(TODAY "Sample 1", "[x.]")
{
  REQUIRE(Solve(1 + R"(
be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe
edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec | fcgedb cgb dgebacf gc
fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef | cg cg fdcagb cbg
fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | efabcd cedba gadfec cb
aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | gecf egdcabf bgf bfgea
fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | gebdcfa ecba ca fadegcb
dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe
bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd | ed bcgafe cdgba cbgef
egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | gbdfcae bgc cg cgb
gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | fgae cfgab fg bagce
)")
            .Do2() == "61229");
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
