// Aoc - 2021 Day ?: ??????? ---
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "10"

#define FIRST_STAR  "271245"
#define SECOND_STAR "1685293086"

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
         views::transform(StaticCast<string>()) | to<vector>;
}
struct Solve
{
  decltype(Init(string())) data;
  map<char, char>          pairs;

  Solve(const string & inStr)
  {
    data = Init(inStr);

    pairs[')'] = '(';
    pairs[']'] = '[';
    pairs['}'] = '{';
    pairs['>'] = '<';
  };

  vector<pair<string, std::deque<char>>> good;

  string Do()
  {
    map<char, int> scores;
    scores[')'] = 3;
    scores[']'] = 57;
    scores['}'] = 1197;
    scores['>'] = 25137;

    vector<char> errors;
    for (auto line : data)
    {
      std::deque<char> stiva;
      auto             errCh = 'a';
      for (auto ch : line)
      {
        if (!scores.contains(ch))
          stiva.push_back(ch);
        else
        {
          if (stiva.back() != pairs[ch])
          {
            auto x = stiva.back();
            auto y = pairs[ch];

            errCh = ch;
            break;
          }
          else
          {
            // continue with the starting bracket poped
            stiva.pop_back();
          }
        }
      }

      if (errCh != 'a')
        errors.push_back(errCh);
      else
        good.emplace_back(line, stiva);
    }

    auto x = accumulate(errors | views::transform(
                                   [&](auto i)
                                   {
                                     return scores[i];
                                   }),
                        0);

    return to_string(x);
  }

  string Do2()
  {
    Do();

    map<char, int> scores;
    scores[')'] = 1;
    scores[']'] = 2;
    scores['}'] = 3;
    scores['>'] = 4;

    auto pairsRev = views::zip(pairs | views::values, pairs | views::keys) | to<map>;

    auto ss = good | views::values |
              views::transform(
                [&](auto stack)
                {
                  return accumulate(stack | views::reverse, 0ull,
                                    [&](auto x, auto ch)
                                    {
                                      return x * 5 + scores[pairsRev[ch]];
                                    });
                }) |
              to<vector>;

    sort(ss);

    return to_string(ss[ss.size() / 2]);
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", "[x.]")
{
  REQUIRE(Solve(1 + R"(
[({(<(())[]>[[{[]{<()<>>
[(()[<>])]({[<{<<[]>>(
{([(<{}[<>[]}>{[]{[(<()>
(((({<>}<{<{<>}{[]{[]{}
[[<[([]))<([[{}[[()]]]
[{[{({}]{}}([{[{{{}}([]
{<[[]]>}<{[{[{[]{()[[[]
[<(<(<(<{}))><([]([]()
<{([([[(<>()){}]>(<<{{
<{([{{}}[<[[[<>{}]]]>[]]
)")
            .Do2() == "288957");
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
