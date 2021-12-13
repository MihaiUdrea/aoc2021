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
  auto r = input | views::tokenize(regex("\n"), -1) | views::transform(to_string_view()) |
           views::transform(
             [](auto line)
             {
               auto x = line | views::tokenize(regex("\\w+")) | to<vector<string>>;

               return vector{ make_pair(at(x, 0), at(x, 1)), make_pair(at(x, 1), at(x, 0)) };
             }) |
           to<vector>;

  auto q = r | views::join | to<vector>;
  sort(q,
       [](auto a, auto b)
       {
         return a.first < b.first;
       });

  return q |
         views::chunk_by(
           [](auto a, auto b)
           {
             return a.first == b.first;
           }) |
         views::transform(
           [](auto r)
           {
             auto rgn = r | views::values | to<set>;
             return make_pair(r[0].first, rgn);
           }) |
         to<map>;
}
struct Solve
{
  decltype(Init(string())) data;

  Solve(const string & inStr) { data = Init(inStr); };

  std::deque<string> stiva;

  bool isLowerCase(const string & s)
  {
    if (s[0] >= 'A' && s[0] <= 'Z')
      return false;
    return true;
  }

  int Calc2()
  {
    auto crNode = stiva.back();

    int count = 0;
    for (auto n : data[crNode])
    {
      bool dubchanged = false;
      if (n == "end")
      {
        count += 1;
        for (auto s : stiva)
          cout << s << " ";
        cout << endl;
      }
      else
      {
        if (isLowerCase(n) && contains(stiva, n))
        {
          if (dub || n == "start")
            continue;
          else
          {
            dub        = true;
            dubchanged = true;
          }
        }

        stiva.push_back(n);
        count += Calc2();
        stiva.pop_back();
        if (dubchanged)
        {
          dub = false;
        }
      }
    }
    return count;
  }

  int Calc()
  {
    auto crNode = stiva.back();

    int count = 0;
    for (auto n : data[crNode])
    {
      if (n == "end")
      {
        count += 1;
      }
      else
      {
        if (isLowerCase(n) && contains(stiva, n))
          continue;

        stiva.push_back(n);
        count += Calc();
        stiva.pop_back();
      }
    }
    return count;
  }

  bool   dub{ false };
  string Do()
  {
    stiva.push_back("start");

    auto x = data | views::tail;

    return to_string(Calc());
  }

  string Do2()
  {
    stiva.push_back("start");
    return to_string(Calc2());
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", "[x.]")
{
  REQUIRE(Solve(1 + R"(
start-A
start-b
A-c
A-b
b-d
A-end
b-end
)")
            .Do2() == "36");
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
