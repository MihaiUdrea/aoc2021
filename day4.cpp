// Aoc - 2021 Day ?: ??????? ---
#include "stdafx.h"
#include "Utils.h"

//#define THISDAY "?"

#define FIRST_STAR  "71708"
#define SECOND_STAR "34726"

#ifdef THISDAY
#define TODAY DAY THISDAY "/"
#else
#define TODAY
#endif  // THISDAY

using namespace ranges;

// Flattens a range of ranges by iterating the inner
// ranges in round-robin fashion.
template <class Rngs>
class interleave_view : public view_facade<interleave_view<Rngs>>
{
  friend range_access;
  std::vector<range_value_t<Rngs>> rngs_;
  struct cursor;
  cursor begin_cursor()
  {
    return { 0, &rngs_, views::transform(rngs_, ranges::begin) | to<std::vector> };
  }

public:
  interleave_view() = default;
  explicit interleave_view(Rngs rngs)
    : rngs_(std::move(rngs) | to<std::vector>)
  {
  }
};

template <class Rngs>
struct interleave_view<Rngs>::cursor
{
  std::size_t                                  n_;
  std::vector<range_value_t<Rngs>> *           rngs_;
  std::vector<iterator_t<range_value_t<Rngs>>> its_;
  decltype(auto)                               read() const { return *its_[n_]; }
  void                                         next()
  {
    if (0 == ((++n_) %= its_.size()))
      for_each(its_,
               [](auto & it)
               {
                 ++it;
               });
  }
  bool equal(default_sentinel_t) const
  {
    if (n_ != 0)
      return false;
    auto ends = *rngs_ | views::transform(ranges::end);
    return its_.end() !=
           std::mismatch(its_.begin(), its_.end(), ends.begin(), std::not_equal_to<>{}).first;
  }
  CPP_member auto equal(cursor const & that) const
    -> CPP_ret(bool)(requires forward_range<range_value_t<Rngs>>)
  {
    return n_ == that.n_ && its_ == that.its_;
  }
};

// In:  range<range<T>>
// Out: range<T>, flattened by walking the ranges
//                round-robin fashion.
auto interleave()
{
  return make_view_closure(
    [](auto && rngs)
    {
      using Rngs = decltype(rngs);
      return interleave_view<views::all_t<Rngs>>(views::all(std::forward<Rngs>(rngs)));
    });
}

// In:  range<range<T>>
// Out: range<range<T>>, transposing the rows and columns.
auto transpose()
{
  return make_view_closure(
    [](auto && rngs)
    {
      using Rngs = decltype(rngs);
      CPP_assert(forward_range<Rngs>);
      return std::forward<Rngs>(rngs) | interleave() |
             views::chunk(static_cast<std::size_t>(distance(rngs)));
    });
}

namespace
{

auto Init(const string & input)
{
  auto sections = input | views::tokenize(regex("\n\n"), -1);
  auto first    = (sections | views::take(1) | views::transform(to_string_view()) |
                views::transform(
                     [](auto line)
                     {
                    auto x = line | views::tokenize(regex(","), -1) |
                             views::transform(sub_match_to_int()) | to<vector>;

                    return x;
                     }))
                 .front();

  auto sec = sections | views::tail | views::transform(to_string_view()) |
             views::transform(
               [](auto mat)
               {
                 return mat | views::tokenize(regex("\n"), -1) |
                        views::transform(to_string_view()) |
                        views::transform(
                          [](auto line)
                          {
                            auto x = line | views::tokenize(regex("\\d+")) |
                                     views::transform(sub_match_to_int()) |
                                     views::transform(
                                       [](auto i)
                                       {
                                         return std::make_pair(i, false);
                                       }) |
                                     to<vector>;
                            return x;
                          }) |
                        to<vector>;
               }) |
             to<vector>;

  return make_pair(first, sec);
}
struct Solve
{
  decltype(Init(string())) data;

  Solve(const string & inStr) { data = Init(inStr); };

  string Do()
  {
    auto & [line, tickets] = data;

    for (auto nrDrawn : line)
    {
      // mark as drawn

      for (auto & ticket : tickets)
      {
        for (auto & line : ticket)
        {
          for (auto [colIdx, nr] : line | views::enumerate)
          {
            if (nr.first == nrDrawn)
              nr.second = true;

            auto totalOnLine = count(line | views::values, true);
            if (totalOnLine == line.size())
            {
              // winner
              auto ticketVal = accumulate(ticket | views::join |
                                            views::filter(
                                              [](auto nr)
                                              {
                                                return nr.second;
                                              }) |
                                            views::keys,
                                          0);
              return to_string(ticketVal * nrDrawn);
            }

            // split in columns
            auto rgn = ticket | transpose() | views::drop(colIdx) | views::take(1);
            for (auto col : rgn)
            {
              auto totalOnLine = count_if(col,
                                          [](auto i)
                                          {
                                            return i.second;
                                          });
              if (totalOnLine == line.size())
              {
                // winner
                auto ticketVal = accumulate(ticket | views::join, 0,
                                            [](auto v, auto nr)
                                            {
                                              if (nr.second)
                                                return v;
                                              else
                                                return v + nr.first;
                                              // return v + (? 0 : nr.first);
                                            });
                return to_string(ticketVal * nrDrawn);
              }
            }
          }
        }
      }
    }

    return to_string(1);
  }

  string Do2()
  {
    vector<int> winersScores;
    set<int>    winers;

    auto & [line, tickets] = data;

    for (auto nrDrawn : line)
    {
      // mark as drawn

      for (auto ticketAndIdx : tickets | views::enumerate)
      {
        auto & idx    = ticketAndIdx.first;
        auto & ticket = ticketAndIdx.second;

        if (contains(winers, idx))
          continue;

        bool winn = false;

        for (auto & line : ticket)
        {
          for (auto & nr : line)
          {
            if (nr.first == nrDrawn)
            {
              nr.second = true;

              auto totalOnLine = count_if(line,
                                          [](auto i)
                                          {
                                            return i.second;
                                          });
              if (totalOnLine == line.size())
              {
                // winner
                auto ticketVal = accumulate(ticket | views::join, 0,
                                            [](auto v, auto nr)
                                            {
                                              if (nr.second)
                                                return v;
                                              else
                                                return v + nr.first;
                                              // return v + (? 0 : nr.first);
                                            });
                winersScores.push_back(ticketVal * nrDrawn);
                winers.insert(idx);

                winn = true;
              }

              // split in columns
              if (!winn)
              {
                auto rgn = ticket | transpose();
                for (auto col : rgn)
                {
                  auto totalOnLine = count_if(col,
                                              [](auto i)
                                              {
                                                return i.second;
                                              });
                  if (totalOnLine == line.size())
                  {
                    // winner
                    auto ticketVal = accumulate(ticket | views::join, 0,
                                                [](auto v, auto nr)
                                                {
                                                  if (nr.second)
                                                    return v;
                                                  else
                                                    return v + nr.first;
                                                  // return v + (? 0 : nr.first);
                                                });
                    winersScores.push_back(ticketVal * nrDrawn);
                    winers.insert(idx);

                    winn = true;
                    break;
                  }
                }
              }
            }
            if (winn)
              break;
          }
          if (winn)
            break;
        }
      }
    }

    return to_string(winersScores.back());
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", "[x.]")
{
  REQUIRE(Solve(1 + R"(
7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1

22 13 17 11  0
 8  2 23  4 24
21  9 14 16  7
 6 10  3 18  5
 1 12 20 15 19

 3 15  0  2 22
 9 18 13 17  5
19  8  7 25 23
20 11 10 24  4
14 21 16 12  6

14 21 17 24  4
10 16 15  9 19
18  8 23 26 20
22 11 13  6  5
 2  0 12  3  7
)")
            .Do2() == "1924");
}

TEST_CASE(TODAY "Part One", "[x.]")
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
