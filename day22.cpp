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
             auto x = line | views::tokenize(regex("on|off|(-?\\d+)")) | to<vector>;

             return std::make_tuple(x.front().str() == "on",
                                    Point(stoi(x[1].str()), stoi(x[3].str()), stoi(x[5].str())),
                                    Point(stoi(x[2].str()), stoi(x[4].str()), stoi(x[6].str())));
           }) |
         to<vector>;
}
struct Solve
{
  decltype(Init(string())) data;

  Solve(const string & inStr) { data = Init(inStr); };

  string Brute(decltype(Init(string())) data,
               Point                    topleft  = Point(-50, -50, -50),
               Point                    botRight = Point(50, 50, 50))
  {
    set<Point> onList;
    for (auto cube : data)
    {
      if (get<0>(cube))
      {
        // on

        Point crTopLeft  = topleft;
        Point crBotRight = botRight;

        Point otherTopLeft  = get<1>(cube);
        Point otherBotRight = get<2>(cube);
        if (ManhDist(botRight - topleft) > ManhDist(get<2>(cube) - get<1>(cube)))
        {
          otherTopLeft  = crTopLeft;
          otherBotRight = crBotRight;

          crTopLeft  = get<1>(cube);
          crBotRight = get<2>(cube);
        }
        for (auto i : views::cartesian_product(views::closed_iota(crTopLeft.x, crBotRight.x),
                                               views::closed_iota(crTopLeft.y, crBotRight.y),
                                               views::closed_iota(crTopLeft.z, crBotRight.z)))
        {
          Point pt(get<0>(i), get<1>(i), get<2>(i));

          if (pt.IsInsideRectangle(otherTopLeft, otherBotRight))
          {
            onList.insert(pt);
          }
        }
      }
      else
      {
        // off
        // Point diffCube(get<2>(cube) - get<1>(cube));
        // if (onList.size() > diffCube.Volume())
        auto toRemove = onList |
                        views::filter(
                          [&](auto pt)
                          {
                            return pt.IsInsideRectangle(get<1>(cube), get<2>(cube));
                          }) |
                        to<vector>;
        for (auto pt : toRemove)
          onList.erase(pt);
      }
    }

    return to_string(onList.size());
  }
  string Do() { return Brute(data); }

  struct Cube
  {
    Cube(Point minCorner = {}, Point maxCorner = {})
      : minCorner(minCorner)
      , maxCorner(maxCorner)
    {
    }
    auto operator<=>(const Cube &) const = default;

    Point minCorner{};
    Point maxCorner{};

    auto Volume() { return (maxCorner - minCorner).Volume(); }

    auto GetAllCorners()
    {
      vector<Point> res;
      res.push_back(minCorner);
      Point x(minCorner);
      x.x = maxCorner.x;
      res.push_back(x);
      x.y = maxCorner.y;
      res.push_back(x);
      x.x = minCorner.x;
      res.push_back(x);
      x.z = maxCorner.z;
      res.push_back(x);
      x.y = minCorner.y;
      res.push_back(x);
      res.push_back(maxCorner);
      return res;
    }

    auto GetCorner(bool left, bool bot, bool front)
    {
      Point p;
      p.x = left ? minCorner.x : maxCorner.x;
      p.y = bot ? minCorner.y : maxCorner.y;
      p.z = front ? minCorner.z : maxCorner.z;

      return p;
    }

    auto GetAllCorners2()
    {
      vector truFalse = { false, true };
      return views::cartesian_product(truFalse, truFalse, truFalse) |
             views::transform(
               [this](auto a)
               {
                 return GetCorner(get<0>(a), get<1>(a), get<2>(a));
               }) |
             to<vector>;
    }
  };

  static std::tuple<vector<Cube>, vector<Cube>, Cube> Intersect(Cube smal, Cube bigg)
  {
    vector<Cube> resSmal;
    vector<Cube> resBig;
    Cube         commonPart;

    bool swapped = false;
    if (smal.Volume() > bigg.Volume())
    {
      std::swap(smal, bigg);
      swapped = true;
    }

    return { resSmal, resBig, commonPart };
  }

  enum
  {
    Bot = true,
    Top = false
  };
  enum
  {
    Left  = true,
    Right = false
  };
  enum
  {
    Front = true,
    Back  = false
  };

  static pair<Cube, pair<vector<Cube>, vector<Cube>>> CubeDifference2(Cube bigg, Cube smal)
  {
    pair<vector<Cube>, vector<Cube>> acc;

    auto fullRun = true;
    bool swaped  = false;

    for (auto i : views::indices(3))
    {
      auto compCorner = [i](auto a)
      {
        return a.maxCorner.GetAxys(i) - a.minCorner.GetAxys(i);
      };

      vector<Cube> cubes = { bigg, smal };
      // sort(cubes, greater{}, compCorner);

      // if (cubes[0] != bigg)
      //   swaped = !swaped;

      // midle
      bool sides = false;

      Cube mid0 = cubes[0];
      Cube mid1 = cubes[1];

      if (cubes[1].minCorner.GetAxys(i) <= cubes[0].maxCorner.GetAxys(i) &&
          cubes[1].minCorner.GetAxys(i) >= cubes[0].minCorner.GetAxys(i))
      {
        Cube leftSide = cubes[0];

        // left side
        leftSide.maxCorner.SetAxyx(i, cubes[1].minCorner.GetAxys(i) - 1);

        mid0.minCorner.SetAxyx(i, cubes[1].minCorner.GetAxys(i));

        //(!swaped ? acc.first : acc.second).emplace_back(leftSide);

        if (leftSide.Volume() > 0)
        {
          acc.first.emplace_back(leftSide);

          sides = true;
        }
      }

      if (cubes[1].maxCorner.GetAxys(i) >= cubes[0].minCorner.GetAxys(i) &&
          cubes[1].maxCorner.GetAxys(i) <= cubes[0].maxCorner.GetAxys(i))
      {
        Cube rightSide = cubes[0];

        // right side
        rightSide.minCorner.SetAxyx(i, cubes[1].maxCorner.GetAxys(i) + 1);
        mid0.maxCorner.SetAxyx(i, cubes[1].maxCorner.GetAxys(i));

        if (rightSide.Volume() > 0)
        {
          acc.first.emplace_back(rightSide);
          sides = true;
        }
      }

      if (cubes[0].minCorner.GetAxys(i) <= cubes[1].maxCorner.GetAxys(i) &&
          cubes[0].minCorner.GetAxys(i) >= cubes[1].minCorner.GetAxys(i))
      {
        Cube leftSide = cubes[1];

        // left side
        leftSide.maxCorner.SetAxyx(i, cubes[0].minCorner.GetAxys(i) - 1);

        mid1.minCorner.SetAxyx(i, cubes[0].minCorner.GetAxys(i));

        //(!swaped ? acc.first : acc.second).emplace_back(leftSide);

        if (leftSide.Volume() > 0)
        {
          acc.second.emplace_back(leftSide);

          sides = true;
        }
      }

      if (cubes[0].maxCorner.GetAxys(i) >= cubes[1].minCorner.GetAxys(i) &&
          cubes[0].maxCorner.GetAxys(i) <= cubes[1].maxCorner.GetAxys(i))
      {
        Cube rightSide = cubes[1];

        // right side
        rightSide.minCorner.SetAxyx(i, cubes[0].maxCorner.GetAxys(i) + 1);
        mid1.maxCorner.SetAxyx(i, cubes[0].maxCorner.GetAxys(i));

        if (rightSide.Volume() > 0)
        {
          acc.second.emplace_back(rightSide);
          sides = true;
        }
      }

      if (!sides)
      {
        fullRun = cubes[0] == cubes[1];
        break;
      }

      bigg = mid0;
      smal = mid1;
    }

    if (fullRun)
    {
      if (bigg != smal)
      {
        cout << "bad";
      }

      return { bigg, acc };
    }

    return { Cube(), {} };
  }

  static vector<Cube> CubeDifference(Cube bigg, Cube smal)
  {
    vector<Cube> res;
    // small is included in big

    // front

    Cube LeftTopFront(bigg.GetCorner(Left, Top, Front),
                      smal.GetCorner(Left, Top, Front) + Point(-1, 1, -1));
    // swap y
    std::swap(LeftTopFront.minCorner.y, LeftTopFront.maxCorner.y);
    res.push_back(LeftTopFront);

    Cube RightTopFront(bigg.GetCorner(Right, Top, Front),
                       smal.GetCorner(Right, Top, Front) + Point(1, 1, -1));
    // swap z
    std::swap(RightTopFront.minCorner.z, RightTopFront.maxCorner.z);
    res.push_back(RightTopFront);

    Cube RightBotFront(bigg.GetCorner(Right, Bot, Front),
                       smal.GetCorner(Right, Bot, Front) + Point(1, -1, -1));
    // swap x
    std::swap(RightBotFront.minCorner.x, RightBotFront.maxCorner.x);
    res.push_back(RightBotFront);

    Cube LeftBotFront(bigg.GetCorner(Left, Bot, Front),
                      smal.GetCorner(Left, Bot, Front) - Point(1, 1, 1));
    // swap nothing
    res.push_back(LeftBotFront);

    // back

    Cube LeftTopBack(bigg.GetCorner(Left, Top, Back),
                     smal.GetCorner(Left, Top, Back) + Point(-1, 1, 1));

    // swap yz
    std::swap(LeftTopBack.minCorner.y, LeftTopBack.maxCorner.y);
    std::swap(LeftTopBack.minCorner.z, LeftTopBack.maxCorner.z);
    res.push_back(LeftTopBack);

    Cube RightTopBack(bigg.GetCorner(Right, Top, Back),
                      smal.GetCorner(Right, Top, Back) + Point(1, 1, 1));

    // swap all
    std::swap(RightTopBack.minCorner, RightTopBack.maxCorner);

    res.push_back(RightTopBack);

    Cube RightBotBack(bigg.GetCorner(Right, Bot, Back),
                      smal.GetCorner(Right, Bot, Back) + Point(1, -1, 1));

    // swap x
    std::swap(RightBotBack.minCorner.x, RightBotBack.maxCorner.x);
    std::swap(RightBotBack.minCorner.z, RightBotBack.maxCorner.z);
    res.push_back(RightBotBack);

    Cube LeftBotBack(bigg.GetCorner(Left, Bot, Back),
                     smal.GetCorner(Left, Bot, Back) + Point(-1, -1, 1));

    // swap z
    std::swap(LeftTopBack.minCorner.z, LeftTopBack.maxCorner.z);
    res.push_back(LeftBotBack);

    // edges
    Cube FrontUpEdge(LeftTopFront.GetCorner(Right, Bot, Front) + Point(1, 0, 0),
                     RightTopFront.GetCorner(Left, Top, Back));

    return {};
  }

  string Do2()
  {
    Point minCorner = get<1>(data[0]);
    Point maxCorner = get<2>(data[0]);

    vector<Cube> onList;
    onList.emplace_back(minCorner, maxCorner);

    vector<pair<Cube, int>> doneList;
    // doneList.emplace_back(Cube(minCorner, maxCorner), get<0>(data[0]));

    for (auto [pos, e] : data | views::enumerate)
    {
      Point minCorner = get<1>(e);
      Point maxCorner = get<2>(e);
      Cube  startCube(minCorner, maxCorner);

      vector<pair<Cube, int>> toAdd;

      for (int i = 0; i < doneList.size(); i++)
      {
        auto & doneCube = doneList[i];

        if (doneCube.second == -1)
          continue;

        if (startCube == doneCube.first)
        {
          cout << "equal";
        }

        auto res = CubeDifference2(doneCube.first, startCube);
        if (res.first != Cube())
        {
          int type        = doneCube.second;
          doneCube.second = -1;

          copy(res.second.first | views::transform(
                                    [&](auto a)
                                    {
                                      return std::make_pair(a, type);
                                    }),
               back_inserter(toAdd));
        }
        else
        {
          toAdd.push_back(doneCube);
        }
      }

      // add this cube
      toAdd.emplace_back(startCube, get<0>(e));

      doneList = std::move(toAdd);

      if (0)
      {
        auto res = accumulate(doneList, 0ull, plus{},
                              [](auto cube)
                              {
                                return cube.second == 1 ? cube.first.Volume() : 0;
                              });
        cout << endl << pos << ": " << res;

        if (0)
        {
          // brute comparison
          auto v = Brute(data | views::take(pos + 1) | to<vector>);
          if (v != to_string(res))
          {
            cout << "BAD";
          }
        }
      }

      if (0)
      {
        vector<Cube> newGen;
        if (get<0>(e))
        {
          // on, store it as unique

          vector<Cube> crCubeList;
          crCubeList.push_back(startCube);

          for (auto onCube : onList)
          {
            vector<Cube> newCubes;
            for (auto i = 0; i < crCubeList.size(); i++)
            {
              auto crCube = crCubeList[i];

              // for (auto onCube : onList)
              {
                auto res = CubeDifference2(onCube, crCube);
                if (res.first != Cube())
                {
                  // something in common

                  crCubeList.erase(crCubeList.begin() + i);
                  copy(res.second.second, back_inserter(crCubeList));
                  break;
                }
                else
                {
                  // nothin in common
                }
              }
            }
          }

          // newCubes are now realy new, add them
          copy(crCubeList, inserter(onList, onList.begin()));
        }
        else
        {
          // off
        }
      }
    }

    return to_string(accumulate(doneList, 0ull, plus{},
                                [](auto cube)
                                {
                                  return cube.second == 1 ? cube.first.Volume() : 0;
                                }));
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "T Diff", "[.]")
{
  REQUIRE(Solve::Cube(Point(0, 0, 0), Point(9, 8, 7)).GetCorner(true, false, false) ==
          Point(0, 8, 7));
  REQUIRE(Solve::Cube(Point(0, 0, 0), Point(9, 8, 7)).GetCorner(false, false, false) ==
          Point(9, 8, 7));

  REQUIRE(Solve::CubeDifference2(Solve::Cube(Point(0, 0), Point(9, 9)),
                                 Solve::Cube(Point(-1, -1), Point(5, 5)))
            .first.minCorner == Point(0, 0));

  REQUIRE(Solve::CubeDifference2(Solve::Cube(Point(0, 0), Point(9, 9)),
                                 Solve::Cube(Point(-1, -1), Point(5, 5)))
            .first.maxCorner == Point(5, 5));

  REQUIRE(Solve::CubeDifference2(Solve::Cube(Point(0, 0), Point(9, 9)),
                                 Solve::Cube(Point(3, 3), Point(5, 5)))
            .first.maxCorner == Point(5, 5));
  REQUIRE(Solve::CubeDifference2(Solve::Cube(Point(0, 0), Point(9, 9)),
                                 Solve::Cube(Point(3, 3), Point(5, 5)))
            .first.minCorner == Point(3, 3));
  REQUIRE(Solve::CubeDifference2(Solve::Cube(Point(0, 0, 0), Point(9, 9, 9)),
                                 Solve::Cube(Point(3, 3, 3), Point(5, 5, 5)))
            .first.minCorner == Point(3, 3, 3));
  REQUIRE(Solve::CubeDifference2(Solve::Cube(Point(0, 0, 0), Point(9, 9, 9)),
                                 Solve::Cube(Point(3, 3, 3), Point(5, 5, 5)))
            .second.first.size() == 3 * 2);
  REQUIRE(Solve::CubeDifference2(Solve::Cube(Point(3, 3, 3), Point(5, 5, 5)),
                                 Solve::Cube(Point(0, 0, 0), Point(9, 9, 9)))
            .second.second.size() == 3 * 2);
}

TEST_CASE(TODAY "T 1", "[.]")
{
  REQUIRE(get<0>(Solve::Intersect(Solve::Cube(Point(0, 0), Point(4, 4)),
                                  Solve::Cube(Point(3, 3), Point(5, 5))))
            .size() == 2);
}

TEST_CASE(TODAY "Sample 1", "[x.]")
{
  REQUIRE(Solve(1 + R"(
on x=-5..47,y=-31..22,z=-19..33
on x=-44..5,y=-27..21,z=-14..35
on x=-49..-1,y=-11..42,z=-10..38
on x=-20..34,y=-40..6,z=-44..1
off x=26..39,y=40..50,z=-2..11
on x=-41..5,y=-41..6,z=-36..8
off x=-43..-33,y=-45..-28,z=7..25
on x=-33..15,y=-32..19,z=-34..11
off x=35..47,y=-46..-34,z=-11..5
on x=-14..36,y=-6..44,z=-16..29
)")
            .Do2() == "474140");
}

TEST_CASE(TODAY "Sample 2", "[x.]")
{
  REQUIRE(Solve(1 + R"(
on x=-5..47,y=-31..22,z=-19..33
on x=-44..5,y=-27..21,z=-14..35
on x=-49..-1,y=-11..42,z=-10..38
on x=-20..34,y=-40..6,z=-44..1
off x=26..39,y=40..50,z=-2..11
on x=-41..5,y=-41..6,z=-36..8
off x=-43..-33,y=-45..-28,z=7..25
on x=-33..15,y=-32..19,z=-34..11
off x=35..47,y=-46..-34,z=-11..5
on x=-14..36,y=-6..44,z=-16..29
on x=-57795..-6158,y=29564..72030,z=20435..90618
on x=36731..105352,y=-21140..28532,z=16094..90401
on x=30999..107136,y=-53464..15513,z=8553..71215
on x=13528..83982,y=-99403..-27377,z=-24141..23996
on x=-72682..-12347,y=18159..111354,z=7391..80950
on x=-1060..80757,y=-65301..-20884,z=-103788..-16709
on x=-83015..-9461,y=-72160..-8347,z=-81239..-26856
on x=-52752..22273,y=-49450..9096,z=54442..119054
on x=-29982..40483,y=-108474..-28371,z=-24328..38471
on x=-4958..62750,y=40422..118853,z=-7672..65583
on x=55694..108686,y=-43367..46958,z=-26781..48729
on x=-98497..-18186,y=-63569..3412,z=1232..88485
on x=-726..56291,y=-62629..13224,z=18033..85226
on x=-110886..-34664,y=-81338..-8658,z=8914..63723
on x=-55829..24974,y=-16897..54165,z=-121762..-28058
on x=-65152..-11147,y=22489..91432,z=-58782..1780
on x=-120100..-32970,y=-46592..27473,z=-11695..61039
on x=-18631..37533,y=-124565..-50804,z=-35667..28308
on x=-57817..18248,y=49321..117703,z=5745..55881
on x=14781..98692,y=-1341..70827,z=15753..70151
on x=-34419..55919,y=-19626..40991,z=39015..114138
on x=-60785..11593,y=-56135..2999,z=-95368..-26915
on x=-32178..58085,y=17647..101866,z=-91405..-8878
on x=-53655..12091,y=50097..105568,z=-75335..-4862
on x=-111166..-40997,y=-71714..2688,z=5609..50954
on x=-16602..70118,y=-98693..-44401,z=5197..76897
on x=16383..101554,y=4615..83635,z=-44907..18747
off x=-95822..-15171,y=-19987..48940,z=10804..104439
on x=-89813..-14614,y=16069..88491,z=-3297..45228
on x=41075..99376,y=-20427..49978,z=-52012..13762
on x=-21330..50085,y=-17944..62733,z=-112280..-30197
on x=-16478..35915,y=36008..118594,z=-7885..47086
off x=-98156..-27851,y=-49952..43171,z=-99005..-8456
off x=2032..69770,y=-71013..4824,z=7471..94418
on x=43670..120875,y=-42068..12382,z=-24787..38892
off x=37514..111226,y=-45862..25743,z=-16714..54663
off x=25699..97951,y=-30668..59918,z=-15349..69697
off x=-44271..17935,y=-9516..60759,z=49131..112598
on x=-61695..-5813,y=40978..94975,z=8655..80240
off x=-101086..-9439,y=-7088..67543,z=33935..83858
off x=18020..114017,y=-48931..32606,z=21474..89843
off x=-77139..10506,y=-89994..-18797,z=-80..59318
off x=8476..79288,y=-75520..11602,z=-96624..-24783
on x=-47488..-1262,y=24338..100707,z=16292..72967
off x=-84341..13987,y=2429..92914,z=-90671..-1318
off x=-37810..49457,y=-71013..-7894,z=-105357..-13188
off x=-27365..46395,y=31009..98017,z=15428..76570
off x=-70369..-16548,y=22648..78696,z=-1892..86821
on x=-53470..21291,y=-120233..-33476,z=-44150..38147
off x=-93533..-4276,y=-16170..68771,z=-104985..-24507
)")
            .Do2() == "2758514936282235");
}

TEST_CASE(TODAY "Part Two", "[x.]")
{
#ifdef SECOND_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do2() == SECOND_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do2());
#endif
}

TEST_CASE(TODAY "Part One", "[.]")
{
#ifdef FIRST_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do() == FIRST_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do());
#endif  // FIRST_STAR
}
