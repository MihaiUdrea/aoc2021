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
  CharMapLimits mapLimits(input);
  return mapLimits;
}
struct Solve
{
  decltype(Init(string())) data;

  Solve(const string & inStr) { data = Init(inStr); };

  int GetAmpCost(char a)
  {
    if (a < 'A' || a > 'D')
      cout << endl << "BADDDD" << endl;

    return pow(10, a - 'A');
  }

  string Do(const string res)
  {
    auto costa = GetAmpCost('A');
    auto costb = GetAmpCost('B');
    auto costc = GetAmpCost('C');
    auto costd = GetAmpCost('D');

    auto doPrint = [](auto newPt, bool tiny) -> string
    {
      if (0)
        if (!tiny)
          return string();

      set<Point> x = views::set_union(newPt.charMap['.'], newPt.charMap['#']) | to<set>;
      if (tiny)
        x.clear();

      for (auto ch : views::closed_iota('A', 'D' + 5 + 10))
      {
        set<Point> y = views::set_union(x, newPt.charMap[ch]) | to<set>;
        x            = std::move(y);
      }

      auto ss = to2Ds(
        x,
        [](auto & l, auto pos)
        {
          return Point(l.y, l.x);
        },
        [&](auto & l)
        {
          if (0)
            for (auto ch : views::closed_iota('A' + 5, 'D' + 5))
              if (newPt.charMap[ch].contains(l))
                return string() + (char)(ch - 5);

          for (auto ch : views::closed_iota('A' + 10 + 5, 'D' + 10 + 5))
            if (newPt.charMap[ch].contains(l))
              return string() + (char)(ch /* - 5 - 10*/);

          for (auto ch : views::closed_iota('A', 'D'))
            if (newPt.charMap[ch].contains(l))
              return string() + ch;
          if (newPt.charMap['.'].contains(l))
            return string() + '.';
          if (newPt.charMap['#'].contains(l))
            return string() + '#';
          return string() + '?';
        },
        tiny ? to2DsFlags::no_header : to2DsFlags::full_header, ' ', 1);

      return ss;
    };

    CharMapLimits destMap(res);

    auto getRemaining = [&](CharMapLimits minPt) -> int
    {
      int cost = 0;
      for (auto amphypod : views::set_union(
             minPt.charMap['D'],
             views::set_union(minPt.charMap['C'],
                              views::set_union(minPt.charMap['A'], minPt.charMap['B']))))
      {
        char amphypodType = minPt.charMap['A'].contains(amphypod)
                              ? 'A'
                              : (minPt.charMap['B'].contains(amphypod)
                                   ? 'B'
                                   : (minPt.charMap['C'].contains(amphypod) ? 'C' : 'D'));
        int  targetCol    = (amphypodType - 'A' + 1) * 2 + 1;
        cost += abs(targetCol - amphypod.y) * GetAmpCost(amphypodType);
      }

      return cost;
    };

    set<string> hist;
    bool        histOn = true;
    // set<CharMapLimits> hist;
    using Breadpath     = pair<vector<pair<Point, Point>>, vector<ULONGLONG>>;
    using ValueType     = pair<pair<CharMapLimits, Breadpath>, int>;
    auto heapComparator = [&](ValueType left, ValueType right)
    {
      return left.second + getRemaining(left.first.first) >
             right.second + getRemaining(right.first.first);
    };
    std::priority_queue<ValueType, vector<ValueType>, decltype(heapComparator)> priorityque(
      heapComparator);

    for (auto p : data.charMap['#'])
      data.ptMap.erase(p);

    data.charMap['#'].clear();

    priorityque.push({ { data, { {}, {} } }, 0 });
    ULONGLONG minVal = -1;
    ULONGLONG expID  = 0;
    do
    {
      auto [a, cost]              = priorityque.top();
      auto [minPt, pathAndParent] = a;
      auto [path, parentIdx]      = pathAndParent;
      priorityque.pop();

      if (minVal != -1 && cost > minVal)
        return to_string(minVal);

      expID++;

      cout << endl << " EXPANDING FROM COST " << cost << "ID " << expID << endl;
      auto ss = doPrint(minPt, false);
      if (!ss.empty())
        cout << ss << endl;

      if (1)
      {
        auto newPt = minPt;
        if (newPt.charMap['A' + 5 + 10].size() == 4 && newPt.charMap['B' + 5 + 10].size() == 4 &&
            newPt.charMap['C' + 5 + 10].size() == 4 && newPt.charMap['D' + 5 + 10].size() == 4)
        {
          // print path
          cout << endl << "RESULT" << endl;
          cout << endl << cost << endl;

          return to_string(cost);
        }
      }

      // expand
      for (auto vacancy : minPt.charMap['.'])
      {
        if (vacancy.x == 1)
        {
          // verify that is not in the hallway entrance
          if (3 == vacancy.y || 5 == vacancy.y || 7 == vacancy.y || 9 == vacancy.y)
            continue;

          // try to remove a amphipod
          for (auto amphypod : views::set_union(
                 minPt.charMap['D'],
                 views::set_union(minPt.charMap['C'],
                                  views::set_union(minPt.charMap['A'], minPt.charMap['B']))))
          {
            // verify if it can be moved

            // already moved in the free spot
            bool free = true;

            if (amphypod.x == 1)
            {
              continue;

              // cout << "on line";
              //  continue;
              auto inc = (vacancy.y - amphypod.y) / abs((amphypod.y - vacancy.y));

              for (int linePtY = amphypod.y + inc; linePtY != vacancy.y; linePtY += inc)
              {
                Point hallpoint = vacancy;
                hallpoint.y     = linePtY;

                if (!minPt.charMap['.'].contains(hallpoint))
                {
                  free = false;
                  break;
                }
              }
            }
            else
            {
              // verify that amphypod on last line has free space to this position
              for (int linePtY = amphypod.y; linePtY != vacancy.y;
                   linePtY += (vacancy.y - amphypod.y) / abs((amphypod.y - vacancy.y)))
              {
                Point hallpoint = vacancy;
                hallpoint.y     = linePtY;

                if (!minPt.charMap['.'].contains(hallpoint))
                {
                  free = false;
                  break;
                }
              }
            }
            if (free)
            {
              for (auto linePtX : views::iota(vacancy.x, amphypod.x))
              {
                Point roompoint = amphypod;
                roompoint.x     = linePtX;

                if (!minPt.charMap['.'].contains(roompoint))
                {
                  free = false;
                  break;
                }
              }
            }

            if (!free)
              continue;

            CharMapLimits newPt = minPt;
            newPt.charMap['.'].erase(vacancy);
            newPt.charMap['.'].insert(amphypod);

            char amphypodType = minPt.charMap['A'].contains(amphypod)
                                  ? 'A'
                                  : (minPt.charMap['B'].contains(amphypod)
                                       ? 'B'
                                       : (minPt.charMap['C'].contains(amphypod) ? 'C' : 'D'));
            newPt.charMap[amphypodType].erase(amphypod);
            newPt.charMap[amphypodType /*+5*/].insert(vacancy);

            // if (hist.contains(newPt))
            //            continue;

            //          hist.insert(newPt);

            int newcost = cost + ManhDist(vacancy - amphypod) * GetAmpCost(amphypodType);

            if (1)
            {
              auto ss    = doPrint(newPt, true);
              auto ssBig = doPrint(newPt, false);
              if (!ssBig.empty())
                cout << endl << "Cost " << newcost << endl << ssBig << endl;

              if (histOn)
                if (hist.contains(ss))
                {
                  continue;
                }
                else
                {
                  hist.insert(ss);
                }
            }

            Breadpath newpath = pathAndParent;
            newpath.first.emplace_back(vacancy, amphypod);
            newpath.second.push_back(expID);

            priorityque.push({ { newPt, newpath }, newcost });
          }

          // hist.insert(newPt);
          // priorityque.push({ newPt, cost + myMap[newPt] });
          //
          // if (newPt == limi.second)
          //{
          //   cout << "Iteration count: " << count << endl;
          //   return to_string(cost + myMap[newPt]);
          // }
        }
        else
        {
          auto colType  = (vacancy.y - 1) / 2;
          auto amphType = colType - 1 + 'A' /* + 5*/;

          if ((vacancy.x == 2 &&
               minPt.charMap[amphType + 5 + 10].contains(vacancy + Point(+1, 0))) ||
              (vacancy.x == 3 && minPt.charMap['.'].contains(vacancy + Point(-1, 0)) &&
               minPt.charMap[amphType + 5 + 10].contains(vacancy + Point(+1, 0)) &&
               minPt.charMap[amphType + 5 + 10].contains(vacancy + Point(+2, 0))) ||
              (vacancy.x == 4 && minPt.charMap['.'].contains(vacancy + Point(-1, 0)) &&
               minPt.charMap['.'].contains(vacancy + Point(-2, 0)) &&
               minPt.charMap[amphType + 5 + 10].contains(vacancy + Point(+1, 0))) ||
              (vacancy.x == 5 && minPt.charMap['.'].contains(vacancy + Point(-1, 0)) &&
               minPt.charMap['.'].contains(vacancy + Point(-2, 0)) &&
               minPt.charMap['.'].contains(vacancy + Point(-3, 0))))
          {
            // verify first pos is taken;
            {
              // try to insert a amphipod of corect type
              auto colType  = (vacancy.y - 1) / 2;
              auto amphType = colType - 1 + 'A' /*+ 5*/;
              for (auto amphypod : minPt.charMap[amphType])
              {
                // verify if it is in the hallway
                if (amphypod.x != 1)
                  continue;

                // verify that amphypod on last line has free space to this position
                bool free = true;
                for (int linePtY = vacancy.y; linePtY != amphypod.y;
                     linePtY += (amphypod.y - vacancy.y) / abs((amphypod.y - vacancy.y)))
                {
                  Point hallpoint = amphypod;
                  hallpoint.y     = linePtY;

                  if (!minPt.charMap['.'].contains(hallpoint))
                  {
                    free = false;
                    break;
                  }
                }
                if (free)
                {
                  for (auto linePtX : views::iota(amphypod.x, vacancy.x))
                  {
                    Point roompoint = vacancy;
                    roompoint.x     = linePtX;

                    if (!minPt.charMap['.'].contains(roompoint))
                    {
                      free = false;
                      break;
                    }
                  }
                }

                if (!free)
                  continue;

                CharMapLimits newPt = minPt;
                newPt.charMap['.'].erase(vacancy);
                newPt.charMap['.'].insert(amphypod);

                newPt.charMap[amphType].erase(amphypod);
                newPt.charMap[amphType + 5 + 10].insert(vacancy);

                // if (hist.contains(newPt))
                //            continue;

                //          hist.insert(newPt);

                int newcost = cost + ManhDist(vacancy - amphypod) * GetAmpCost(amphType /* - 5*/);

                if (1)
                  if (newPt.charMap['A' + 5 + 10].size() == 4 &&
                      newPt.charMap['B' + 5 + 10].size() == 4 &&
                      newPt.charMap['C' + 5 + 10].size() == 4 &&
                      newPt.charMap['D' + 5 + 10].size() == 4)
                  {
                    if (minVal == -1 && minVal > newcost)
                    {
                      minVal = newcost;
                      // MessageBoxA(0, "min", "min", 0);
                    }

                    // print path
                    Breadpath newpath = pathAndParent;
                    newpath.first.emplace_back(vacancy, amphypod);
                    newpath.second.push_back(expID);

                    cout << "DONE" << endl << newcost << endl;

                    for (auto [vac, amp] : newpath.first)
                    {
                      cout << endl
                           << "  vac.x " << vac.x << "  vac.y " << vac.y << "  amp.x " << amp.x
                           << "  amp.y " << amp.y;
                    }

                    for (auto idx : newpath.second)
                    {
                      cout << endl << "  IDX: " << idx;
                    }

                    // return to_string(newcost);
                  }

                //
                if (1)
                {
                  auto ss    = doPrint(newPt, true);
                  auto ssBig = doPrint(newPt, false);
                  if (!ssBig.empty())
                    cout << endl << "Cost " << newcost << endl << ssBig << endl;

                  if (histOn)
                    if (hist.contains(ss))
                    {
                      continue;
                    }
                    else
                    {
                      hist.insert(ss);
                    }
                }

                Breadpath newpath = pathAndParent;
                newpath.first.emplace_back(vacancy, amphypod);
                newpath.second.push_back(expID);

                priorityque.push({ { newPt, newpath }, newcost });
              }
            }
          }
        }
      }
    } while (true);

    return to_string(minVal);
  }

  string Do2()
  {
    data;
    return to_string(1);
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", "[.]")
{
  REQUIRE(Solve(1 + R"(
#############
#...........#
###B#C#B#D###
  #P#D#R#A#
  #########
)")
            .Do(1 + R"(
#############
#...........#
###A#B#C#D###
  #A#B#C#D#
  #########
)") == to_string(40 + 400 + 3000 + 30 + 40 + 2003 + 7000 + 8));
}

TEST_CASE(TODAY "Part Two", "[.]")
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
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do(1 + R"(
#############
#...........#
###A#B#C#D###
  #A#B#C#D#
  #########
)"));
#endif  // FIRST_STAR
}
