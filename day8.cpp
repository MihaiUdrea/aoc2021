// Aoc - 2021 Day ?: ??????? ---
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "8"

#define FIRST_STAR  "449"
#define SECOND_STAR "968175"

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
             auto sets = line | views::tokenize(regex("\\|"), -1) |
                         views::transform(to_string_view()) |
                         views::transform(
                           [](auto section)
                           {
                             return section | views::tokenize(regex("\\w+")) |
                                    views::transform(
                                      [](auto token)
                                      {
                                        string s = token.str();
                                        sort(s);
                                        return s;
                                      }) |
                                    to<vector>;
                           }) |
                         to<vector>;

             return std::make_pair(sets.front(), sets.back());
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

    auto filter = [&](auto a0)
    {
      auto [a, b] = a0;

      // auto digStr = a |
      //               views::filter(
      //                 [&](auto s)
      //                 {
      //                   return contains(req, s.length());
      //                 }) |
      //               views::transform(
      //                 [&](auto s)
      //                 {
      //                   return std::make_pair(revDigReq[s.length()], s);
      //                 }) |
      //               to<map>;

      map<int, string> digStr;
      for (auto s : a)
      {
        if (contains(req, s.length()))
        {
          digStr[revDigReq[s.length()]] = s;
        }
      }

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

      digStr[6] = *find_if(rgn6,
                           [&](auto s)
                           {
                             return distance(views::set_difference(digStr[1], s)) != 0;
                           });

      for (auto s : rgn6)
      {
        if (contains(digStr | views::values, s))
          continue;

        if (distance(views::set_difference(digStr[4], s)) != 0)
          digStr[0] = s;
        else
          digStr[9] = s;
      }

      for (auto s : rgn5)
      {
        if (distance(views::set_difference(digStr[1], s)) == 0)
          digStr[3] = s;
      }

      for (auto s : rgn5)
      {
        if (contains(digStr | views::values, s))
          continue;

        if (auto dist = distance(views::set_difference(digStr[6], s)); dist == 1)
          digStr[5] = s;
        else if (dist == 2)
          digStr[2] = s;
      }

      auto revDigStr = digStr |
                       views::transform(
                         [](auto e)
                         {
                           return std::make_pair(e.second, e.first);
                         }) |
                       to<map<string, int>>;

      return accumulate(b, 0,
                        [&revDigStr](auto nr, auto s)
                        {
                          return nr * 10 + revDigStr[s];
                        });
    };

    int sum = accumulate(data | views::transform(filter), 0);

    return to_string(sum);
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
