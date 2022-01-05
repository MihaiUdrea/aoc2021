// Aoc - 2021 Day ?: ??????? ---
#include "stdafx.h"
#include "Utils.h"
#include <execution>

//#define THISDAY "?"

//#define FIRST_STAR  ""
//#define SECOND_STAR ""

#ifdef THISDAY
#define TODAY DAY THISDAY "/"
#else
#define TODAY
#endif  // THISDAY

template <typename Iterator>
Iterator find_next_increase_position(Iterator begin, Iterator combination_end, Iterator end);

template <typename Iterator>
bool next_combination(Iterator begin, Iterator end, unsigned k)
{
  const auto combination_end = begin + k;
  const auto next_move       = find_next_increase_position(begin, combination_end, end);
  if (next_move == end)
    return false;
  const auto previous_value = *next_move;
  std::inplace_merge(next_move, combination_end, end);
  const auto next_rotation =
    std::rotate(next_move, std::upper_bound(next_move, end, previous_value), end);
  std::rotate(combination_end, next_rotation, end);
  return true;
}

template <typename Iterator>
Iterator find_next_increase_position(Iterator begin, Iterator combination_end, Iterator end)
{
  auto pos = std::upper_bound(std::reverse_iterator(combination_end), std::reverse_iterator(begin),
                              *--end, std::greater<typename Iterator::value_type>());
  if (pos.base() == begin)
    return ++end;
  return --pos.base();
}

// template <class RandIt, class Compare>
// bool next_combination(RandIt first, RandIt mid, RandIt last, Compare comp)
// {
//   std::sort(mid, last, std::bind(comp, std::placeholders::_2, std::placeholders::_1));
//   return std::next_permutation(first, last, comp);
// }

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
  return input | views::tokenize(regex("\n\n"), -1) | views::transform(to_string_view()) |
         views::transform(
           [](auto chunk)
           {
             return chunk | views::tokenize(regex("\n"), -1) | views::drop(1) |
                    views::transform(to_string_view()) |
                    views::transform(
                      [](auto line)
                      {
                        auto y = line | views::tokenize(regex(","), -1) | to<vector>;

                        return Point(stoi(y[0].str()) * 2, stoi(y[1].str()) * 2,
                                     stoi(y[2].str()) * 2);
                      }) |
                    to<vector>;
           }) |
         to<vector>;
}
struct Solve
{
  decltype(Init(string())) data;

  Solve(const string & inStr) { data = Init(inStr); };

  static auto Rotate3D(Point pt, Point cnt)
  {
    for (auto i : views::indices(3))
      pt.RotateAxis(90 * cnt.GetAxys(i), i);

    return pt;
  }

  static auto GetAllRotations(Point pt)
  {
    vector<Point> res;

    set<Point> settt;

    auto doInsert = [&](Point pt)
    {
      res.push_back(pt);

      if (settt.contains(pt))
      {
        // cout << "bad";
      }

      settt.insert(pt);
    };

    doInsert(pt);

    for (auto angle0 : views::indices(4))
    {
      for (auto angle1 : views::indices(4))
      {
        // rotate over y

        pt.RotateAxis(90, 1);
        if (angle1 != 3)
          doInsert(pt);
      }
      pt.RotateAxis(90, 2);  // rotate over z
      if (angle0 != 3)
        doInsert(pt);
    }

    pt.RotateAxis(90, 0);  // rotate over x
    doInsert(pt);

    for (auto angle1 : views::indices(4))
    {
      // rotate over y

      pt.RotateAxis(90, 1);
      if (angle1 != 3)
        doInsert(pt);
    }

    pt.RotateAxis(90, 0);  // rotate over x
    pt.RotateAxis(90, 0);  // rotate over x

    doInsert(pt);

    for (auto angle : views::indices(4))
    {
      // rotate over y

      pt.RotateAxis(90, 1);
      if (angle != 3)
        doInsert(pt);
    }

    return res;
  }

  void Print(Point p) { cout << " { " << p.x << "," << p.y << "," << p.z << " }"; }

  bool IsEq(vector<Point> & a, vector<Point> & b)
  {
    if (0)
      return a == b;
    else
    {
      return (a[0] == b[0] && a[1] == b[1]) || (a[0] == b[1] && a[1] == b[0]);
    }
  }

  void Print(vector<Point> & otherSeg)
  {
    Print(otherSeg[0]);
    cout << "  |  ";
    Print(otherSeg[1]);
    cout << endl;
  }

  static Point SplitIdxInRotations(int a)
  {
    Point p;
    p.y = a % 4;

    if (a < 16)
      p.z = a / 4;
    else
      p.z = 0;

    if (a < 16)
      p.x = 0;
    else if (a < 20)
      p.x = 1;
    else
      p.x = 3;

    return p;
  }

  int CombineRotations(int a, int b)
  {
    Point pa = SplitIdxInRotations(a);
    Point pb = SplitIdxInRotations(b);

    return 1;
  }

  string Do()
  {
    int combSize = 2;

    auto twelveComb =
      data |
      views::transform(
        [&](auto scaner)
        {
          vector<pair<vector<Point>, Point>> ret;

          vector<Point> comb = scaner;
          sort(comb);

          do
          {
            vector<Point> newComb(comb.begin(), comb.begin() + combSize);

            // norm
            auto x = minmax(newComb, less{}, &Point::x);
            auto y = minmax(newComb, less{}, &Point::y);
            auto z = minmax(newComb, less{}, &Point::z);

            Point center((x.max.x + x.min.x) / 2, (y.max.y + y.min.y) / 2, (z.max.z + z.min.z) / 2);

            for (auto & p : newComb)
              p = p - center;

            ret.emplace_back(std::move(newComb), center);
          } while (next_combination(comb.begin(), comb.end(), combSize));

          return ret;
        }) |
      to<vector>;

    // MessageBox(0, L"comb", 0, 0);

    auto allComb =
      twelveComb |
      views::transform(
        [&](auto combList)
        {
          return combList |
                 views::transform(
                   [&](auto comb)
                   {
                     auto v = comb.first |
                              views::transform(
                                [&](auto pt)
                                {
                                  return GetAllRotations(pt);
                                }) |
                              to<vector>;

                     auto vsec = GetAllRotations(comb.second);

                     return std::make_pair(v | transpose() | to<vector<vector<Point>>>, vsec);
                   }) |
                 to<vector>;
        }) |
      to<vector>;

    auto dataComb = data |
                    views::transform(
                      [&](auto comb)
                      {
                        auto v = comb |
                                 views::transform(
                                   [&](auto pt)
                                   {
                                     return GetAllRotations(pt);
                                   }) |
                                 to<vector>;

                        return v | transpose() | to<vector<vector<Point>>>;
                      }) |
                    to<vector>;

    map<pair<int, int>, vector<int>>                  ss;
    map<int, vector<std::tuple<Point, int, int>>>     match;
    map<std::tuple<int, int, int>, set<Point>>        match2;
    map<std::tuple<int, int, int, Point>, set<Point>> match3;

    vector<std::tuple<Point, Point, int>> plist = {
      { { -618, -824, -621 }, { -537, -823, -458 }, 0 },
      { { 686, 422, 578 }, { 605, 423, 415 }, 1 },
    };

    if (0)
      for (auto xx : plist)
      {
        auto          p1 = get<0>(xx);
        auto          p2 = get<1>(xx);
        vector<Point> l  = { p1, p2 };
        sort(l);

        for (auto [pos, comb] : twelveComb[get<2>(xx)] | views::enumerate)
        {
          auto v = comb.first;

          for (auto & p : v)
            p = (p + comb.second) / 2;

          sort(v);
          if (l == v)
          {
            cout << "Scanner " << get<2>(xx) << " { " << p1.x << "," << p1.y << "," << p1.z
                 << "         " << p2.x << "," << p2.y << "," << p2.z << "} combIdx = " << pos
                 << endl;
          }
        }
      }

    vector<int> scannerSegRepCount(data.size());

    auto rgn = views::indices(twelveComb.size());
    std::for_each(std::execution::par, rgn.begin(), rgn.end(),
                  [&](auto pos)
                  {
                    for (auto [combIdx, seg] : twelveComb[pos] | views::enumerate)
                    {
                      for (auto posOther : views::indices(allComb.size()))
                      {
                        if (pos == posOther)
                          continue;

                        auto & otherSegList = allComb[posOther];

                        int x = 0;
                        for (auto [otherCombIdx, otherSegList2] : otherSegList | views::enumerate)
                        {
                          int y = 0;
                          y++;
                          if (false && pos == 0 && combIdx == 130 && posOther == 1 &&
                              otherCombIdx == 280)
                          {
                            cout << "Pos= " << pos << "combIdx=" << combIdx << endl;
                            Print(seg.first);
                            cout << "other" << endl;
                            for (auto [dirIdx, otherSeg] : otherSegList2.first | views::enumerate)
                            {
                              Print(otherSeg);
                              cout << endl;
                            }
                          }

                          for (auto [dirIdx, otherSeg] : otherSegList2.first | views::enumerate)
                          {
                            // if (otherSeg == seg.first)
                            if (IsEq(otherSeg, seg.first))
                            {
                              // if (x != 0)
                              //   MessageBox(0, 0, 0, 0);
                              // x++;
                              scannerSegRepCount[pos]++;
                              ss[{ pos, combIdx }].push_back(posOther);
                              match[pos].push_back(std::make_tuple(seg.second, posOther, dirIdx));

                              for (auto p : seg.first)
                                match2[{ pos, posOther, dirIdx }].insert(p + seg.second);
                              for (auto p : seg.first)
                                match3[{ pos, posOther, dirIdx,
                                         (seg.second - otherSegList2.second[dirIdx]) / 2 }]
                                  .insert((p + seg.second) / 2);
                            }
                          }
                        }
                      }
                    }
                  });

    map<int, Point> scannersOffset;
    map<int, Point> scannersOffset2;
    map<int, Point> scannerParentOffset;
    map<int, int>   scannersDirs;
    map<int, int>   scannerParent;

    map<int, int> scannersInitialDir;

    scannersOffset[0] = {};
    set<int> done;

    for (;;)
    {
      auto rgn = views::set_difference(scannersOffset | views::keys, done);
      if (distance(rgn) == 0)
        break;

      int crPos = rgn.front();

      for (auto p : match3)
      {
        if (get<0>(p.first) == crPos)
        {
          if (p.second.size() == 12)
          {
            if (!scannersOffset.contains(get<1>(p.first)))
            {
              auto newScannerId = get<1>(p.first);

              auto pt                           = get<3>(p.first);
              scannerParentOffset[newScannerId] = pt;

              auto absPt = ToAbsolute(pt, crPos, scannersDirs, scannerParentOffset, scannerParent);

              if (scannersDirs.contains(crPos))
              {
                auto rotations = GetAllRotations(pt);
                pt             = rotations[scannersDirs[crPos]];

                // auto p = Rotate3D(pt, SplitIdxInRotations(scannersDirs[crPos]));
              }

              scannersOffset[newScannerId]  = scannersOffset[crPos] + pt;
              scannersOffset2[newScannerId] = absPt;
              scannersDirs[newScannerId]    = get<2>(p.first);
              scannerParent[newScannerId]   = crPos;
              // scannersInitialDir[newScannerId] =
            }
          }
        }
      }
      done.insert(crPos);
    }

    int cnt = 0;
    for (auto p : match3)
    {
      if (p.second.size() == 12)
      {
        cnt++;
      }
    }

    if (0)
    {
      cout << "Total 12 = " << cnt;

      auto ptsCnt = distance(data | views::join);
      cout << "Total pts = " << ptsCnt;

      MessageBox(0, 0, 0, 0);

      set<Point> pts;
      for (auto pos : { 0, 1 })
      {
        auto dirIDx = scannersDirs[pos];

        auto & list = dataComb[pos][dirIDx];
        for (auto pt : list)
          pts.insert(pt / 2 + scannersOffset[pos]);
      }

      cout << pts.size() << endl;
    }

    set<Point> pts;
    for (auto pos : views::indices(data.size()))
    {
      auto & list = data[pos];
      for (auto pt : list)
      {
        auto absPt = ToAbsolute(pt / 2, pos, scannersDirs, scannerParentOffset, scannerParent);

        pts.insert(absPt);
      }
    }

    ULONGLONG max      = 0;
    scannersOffset2[0] = {};
    for (auto [p1, p2] : views::cartesian_product(scannersOffset2, scannersOffset2))
    {
      //  cout << 1;
      auto dist = ManhDist(p2.second - p1.second);
      if (dist > max)
        max = dist;
    }

    return to_string(max);

    // return to_string(pts.size());

    auto twelveSize = count_if(match2 | views::values,
                               [](auto a)
                               {
                                 return a.size() == 12;
                               });

    cout << "each scanner seg rep count" << endl;
    for (auto a : scannerSegRepCount)
      cout << a << endl;

    cout << "total: = " << accumulate(scannerSegRepCount, 0ull, plus{});

    MessageBox(0, L"done", 0, 0);

    for (auto [firstIdx, secondIdx] :
         views::cartesian_product(views::indices(data.size()), views::indices(data.size())))
    {
      if (firstIdx == secondIdx)
        continue;

      auto & firstBeaconsList  = allComb[firstIdx];
      auto & secondBeaconsList = allComb[secondIdx];

      // auto inter = views::indices(24) |
      //              views::for_each(
      //                [&](auto idx)
      //                {
      //                  auto otherBeaconsList = allComb[secondIdx][idx];
      //                  sort(otherBeaconsList);
      //
      //                  auto inter =
      //                    views::set_intersection(firstBeaconsList, otherBeaconsList) |
      //                    to<vector>;
      //                  return yield(inter);
      //                }) |
      //              to<vector>;
      // auto maxSize = max_element(inter, less{}, &vector<Point>::size);
      //// if (maxSize->size() >= combSize)
      //{
      //  cout << maxSize->size() << endl;
      //}
    }

    return to_string(1);
  }

  static Point ToAbsolute(
    auto pt, int crScanner, auto & scannersDirs, auto & scannersOffset, auto & scannerParent)
  {
    if (crScanner != 0 && scannersDirs.contains(crScanner))
    {
      auto rotations = GetAllRotations(pt);
      pt             = rotations[scannersDirs[crScanner]];
      return ToAbsolute(scannersOffset[crScanner] + pt, scannerParent[crScanner], scannersDirs,
                        scannersOffset, scannerParent);
    }
    else
      return pt;
  }

  string Do2()
  {
    auto x = data | views::tail;

    return to_string(1);
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "ROTATE z axis", "[x.]")
{
  Point pt(0, 3, 0);
  pt.RotateAxis(90);
  REQUIRE(pt == Point{ -3, 0, 0 });
  pt.RotateAxis(90);
  REQUIRE(pt == Point{ 0, -3, 0 });
  pt.RotateAxis(90);
  REQUIRE(pt == Point{ 3, 0, 0 });
  pt.RotateAxis(90);
  REQUIRE(pt == Point{ 0, 3, 0 });
}

TEST_CASE(TODAY "ROTATE x axis", "[x.]")
{
  Point pt(0, 3, 0);
  pt.RotateAxis(90, 0);
  REQUIRE(pt == Point{ 0, 0, 3 });
  pt.RotateAxis(90, 0);
  REQUIRE(pt == Point{ 0, -3, 0 });
  pt.RotateAxis(90, 0);
  REQUIRE(pt == Point{ 0, 0, -3 });
  pt.RotateAxis(90, 0);
  REQUIRE(pt == Point{ 0, 3, 0 });
}

TEST_CASE(TODAY "ROTATE y axis", "[x.]")
{
  Point pt2(1, 2, 3);
  auto  x = Solve::GetAllRotations(pt2);
  REQUIRE(x.size() == 24);

  auto pt3drot = Solve::Rotate3D(pt2, { 0, 0, 1 });
  REQUIRE(x[4] == pt3drot);

  if (0)
    for (auto [idx, p] : x | views::enumerate)
    {
      auto pt3drot = Solve::Rotate3D(pt2, Solve::SplitIdxInRotations(idx));

      REQUIRE(p == pt3drot);
    }

  Point pt(3, 0, 0);
  pt.RotateAxis(90, 1);
  REQUIRE(pt == Point{ 0, 0, -3 });
  pt.RotateAxis(90, 1);
  REQUIRE(pt == Point{ -3, 0, 0 });
  pt.RotateAxis(90, 1);
  REQUIRE(pt == Point{ 0, 0, 3 });
  pt.RotateAxis(90, 1);
  REQUIRE(pt == Point{ 3, 0, 0 });
}

TEST_CASE(TODAY "Sample 1", "[x.]")
{
  REQUIRE(Solve(1 + R"(
--- scanner 0 ---
404,-588,-901
528,-643,409
-838,591,734
390,-675,-793
-537,-823,-458
-485,-357,347
-345,-311,381
-661,-816,-575
-876,649,763
-618,-824,-621
553,345,-567
474,580,667
-447,-329,318
-584,868,-557
544,-627,-890
564,392,-477
455,729,728
-892,524,684
-689,845,-530
423,-701,434
7,-33,-71
630,319,-379
443,580,662
-789,900,-551
459,-707,401

--- scanner 1 ---
686,422,578
605,423,415
515,917,-361
-336,658,858
95,138,22
-476,619,847
-340,-569,-846
567,-361,727
-460,603,-452
669,-402,600
729,430,532
-500,-761,534
-322,571,750
-466,-666,-811
-429,-592,574
-355,545,-477
703,-491,-529
-328,-685,520
413,935,-424
-391,539,-444
586,-435,557
-364,-763,-893
807,-499,-711
755,-354,-619
553,889,-390

--- scanner 2 ---
649,640,665
682,-795,504
-784,533,-524
-644,584,-595
-588,-843,648
-30,6,44
-674,560,763
500,723,-460
609,671,-379
-555,-800,653
-675,-892,-343
697,-426,-610
578,704,681
493,664,-388
-671,-858,530
-667,343,800
571,-461,-707
-138,-166,112
-889,563,-600
646,-828,498
640,759,510
-630,509,768
-681,-892,-333
673,-379,-804
-742,-814,-386
577,-820,562

--- scanner 3 ---
-589,542,597
605,-692,669
-500,565,-823
-660,373,557
-458,-679,-417
-488,449,543
-626,468,-788
338,-750,-386
528,-832,-391
562,-778,733
-938,-730,414
543,643,-506
-524,371,-870
407,773,750
-104,29,83
378,-903,-323
-778,-728,485
426,699,580
-438,-605,-362
-469,-447,-387
509,732,623
647,635,-688
-868,-804,481
614,-800,639
595,780,-596

--- scanner 4 ---
727,592,562
-293,-554,779
441,611,-461
-714,465,-776
-743,427,-804
-660,-479,-426
832,-632,460
927,-485,-438
408,393,-506
466,436,-512
110,16,151
-258,-428,682
-393,719,612
-211,-452,876
808,-476,-593
-575,615,604
-485,667,467
-680,325,-822
-627,-443,-432
872,-547,-609
833,512,582
807,604,487
839,-516,451
891,-625,532
-652,-548,-490
30,-46,-14

)")
            .Do() == "3621");
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
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do());
#endif  // FIRST_STAR
}
