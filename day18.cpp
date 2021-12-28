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
  return input | views::tokenize(regex("\n"), -1) | to<vector<string>>;
}
struct Solve
{
  struct Bracket
  {
    Bracket * left{ nullptr };
    Bracket * right{ nullptr };
    Bracket * parent{ nullptr };

    int val{ 0 };

    int GetDepth() { return parent ? parent->GetDepth() + 1 : 0; }

    bool IsLeaf() { return left == nullptr && right == nullptr; }

    int Sum()
    {
      if (IsLeaf())
        return val;
      else
        return 3 * left->Sum() + 2 * right->Sum();
    }

    int Sum2()
    {
      if (IsLeaf())
        return val;
      else
        return left->Sum2() + right->Sum2();
    }

    string Print()
    {
      if (IsLeaf())
      {
        return to_string(val);
      }
      else
      {
        string s;
        s += '[';
        s += left->Print();
        s += ',';
        s += right->Print();
        s += ']';
        return s;
      }
    }
  };

  decltype(Init(string())) data;

  Solve(const string & inStr) { data = Init(inStr); };

  Bracket * ConstructArb(string s)
  {
    if (s.size() == 1)
    {
      auto node = new Bracket();
      node->val = stoi(s);
      return node;
    }

    int depth = 0;
    for (auto [pos, ch] : s | views::enumerate)
    {
      if (ch == '[')
        depth++;
      else if (ch == ']')
        depth--;
      else if (ch == ',' && depth == 1)
      {
        auto node   = new Bracket();
        auto lefts  = s.substr(1, pos - 1);
        auto rights = s.substr(pos + 1, s.size() - pos - 2);
        node->left  = ConstructArb(lefts);
        node->right = ConstructArb(rights);

        node->left->parent  = node;
        node->right->parent = node;
        return node;
      }
    }

    cout << "bad";
    return nullptr;
  }

  Bracket * Add(Bracket * left, Bracket * right)
  {
    auto * node   = new Bracket;
    node->left    = left;
    node->right   = right;
    left->parent  = node;
    right->parent = node;

    return node;
  }

  void Print(Bracket * node, string s = {}, int step = -1)
  {
    return;

    if (step != -1)
      cout << endl << s << step << endl;

    if (node->IsLeaf())
    {
      cout << node->val;
    }
    else
    {
      cout << '[';
      Print(node->left);
      cout << ',';
      Print(node->right);
      cout << ']';
    }
  }

  string Reduce(string s)
  {
    // cout << endl << s;
    auto nrChar = [](auto ch)
    {
      return ch != ',' && ch != '[' && ch != ']';
    };

    bool keepGooing = false;

    do
    {
      int             depth       = 0;
      int             leftCharPos = -1;
      std::stack<int> openbracketStack;
      std::stack<int> openbracketStackSep;
      keepGooing = false;
      for (int pos = 0; pos < s.size(); pos++)
      {
        auto ch = s[pos];
        if (ch == ',')
        {
          openbracketStackSep.push(pos);
        }
        else if (ch == '[')
        {
          openbracketStack.push(pos);
          depth++;
        }
        else if (ch == ']')
        {
          depth--;
          auto openbracketPos = openbracketStack.top();
          openbracketStack.pop();
          auto sepPos = openbracketStackSep.top();

          openbracketStackSep.pop();

          if (depth >= 4)
          {
            // cout << endl << "explode";

            auto endPos = pos;

            pos = openbracketPos;
            // explode
            if (endPos - pos != 4)
            {
              cout << "bad";
            }

            int left  = stoi(s.substr(pos + 1, sepPos - pos - 1));
            int right = stoi(s.substr(sepPos + 1, endPos - sepPos - 1));

            leftCharPos = -1;
            for (int i = pos - 1; i > 0; i--)
              if (nrChar(s[i]))
              {
                leftCharPos = i;
                break;
              }

            if (leftCharPos > 0)
              s[leftCharPos] += left;

            // find rightpos
            auto nextCharPos = s.find_first_not_of("[],", endPos);
            if (nextCharPos != s.npos)
              s[nextCharPos] += right;
            s.replace(pos, endPos - pos + 1, "0");
            keepGooing = true;

            // cout << endl << s;
            //  pos++;
            //  break;
          }
        }
      }

      if (!keepGooing)
      {
        string s;
        int    startPos = -1;
        for (auto [pos, ch] : s | views::enumerate)
        {
          if (ch >= '0' && ch <= '0')
          {
            if (startPos == -1)
              startPos = pos;
            s += ch;
          }
          else if (s.size() > 1 && stoi(s) >= 10)
          {
            pos         = startPos;
            leftCharPos = -1;
            for (int i = pos - 1; i > 0; i--)
              if (nrChar(s[i]))
              {
                leftCharPos = i;
                break;
              }

            int val = stoi(s);
            {
              // split
              // cout << endl << "split";
              int    left  = val / 2;
              int    right = (val + 1) / 2;
              string newStr("[");
              newStr += to_string(left);
              newStr += ',';
              newStr += to_string(right);
              newStr += ']';
              s.replace(pos, s.size(), newStr);
              keepGooing = true;
              break;
            }
          }
        }
      }
      // cout << endl << s;
    } while (keepGooing);

    return s;
  }

  Bracket * Explode(Bracket * node)
  {
    if (!node)
      return nullptr;

    auto depth = node->GetDepth();
    if (depth >= 4 && node->left && node->left->IsLeaf() && node->right && node->right->IsLeaf())
    {
      // do explode
      int left  = node->left->val;
      int right = node->right->val;
      BubbleUpLeft(node->parent, node, left, true);
      BubbleUpLeft(node->parent, node, right, false);

      node->val = 0;
      delete node->left;
      delete node->right;
      node->left  = nullptr;
      node->right = nullptr;

      return node;
    }
    else if (auto res = Explode(node->left); res)
      return res;
    else
      return Explode(node->right);
  }

  bool BubbleUpDown(Bracket * node, int val, bool isLeft)
  {
    if (node->IsLeaf())
    {
      node->val += val;
      return true;
    }

    auto first  = isLeft ? node->right : node->left;
    auto second = !isLeft ? node->right : node->left;

    if (BubbleUpDown(first, val, isLeft))
      return true;
    return BubbleUpDown(second, val, isLeft);
  }
  bool BubbleUpLeft(Bracket * node, Bracket * from, int val, bool isLeft)
  {
    if (!node)
      return false;

    auto otherChild = isLeft ? node->left : node->right;
    if (otherChild != from)
    {
      auto res = BubbleUpDown(otherChild, val, isLeft);
      if (res)
        return res;
    }

    return BubbleUpLeft(node->parent, node, val, isLeft);
  }

  Bracket * Split(Bracket * node)
  {
    if (!node)
      return nullptr;

    if (node->val >= 10)
    {
      if (!node->IsLeaf())
        cout << "badd";

      // do split
      int left  = node->val / 2;
      int right = (node->val + 1) / 2;

      node->val   = 0;
      node->left  = new Bracket;
      node->right = new Bracket;

      node->left->parent  = node;
      node->right->parent = node;

      node->left->val  = left;
      node->right->val = right;

      return node;
    }
    else if (auto res = Split(node->left); res)
      return res;
    else
      return Split(node->right);
  }

  Bracket * Reduce(Bracket * node)
  {
    Bracket * splitNode    = nullptr;
    Bracket * explodedNode = nullptr;
    int       vv           = 0;
    do
    {
      // cout << endl;
      explodedNode = Explode(node);

      vv++;

      if (!explodedNode)
      {
        splitNode = Split(node);

        // cout << endl << "after split, x=" << vv << " sum2 = " << node->Sum2() << endl;

        Print(node);
      }
      else
      {
        // cout << endl << "after explode, x=" << vv << " sum2 = " << node->Sum2() << endl;
        Print(node);
      }
    } while (explodedNode != nullptr || splitNode != nullptr);

    return node;
  }

  string Do()
  {
    string s    = data[0];
    auto   node = ConstructArb(s);
    Print(node, "init", 0);
    for (int i = 1; i < data.size(); ++i)
    {
      auto * next = ConstructArb(data[i]);
      auto * add  = Add(node, next);
      Print(add, "Add:", i);

      // cout << endl << "before reduce, sum2= " << add->Sum2() << endl;

      node = Reduce(add);
      Print(node, "Reduce: ", i);
    }

    return to_string(node->Sum());
    // return node->Print();
  }

  string Do1()
  {
    string s = data[0];
    for (int i = 1; i < data.size(); ++i)
    {
      s = Reduce(std::string() + '[' + s + ',' + data[i] + ']');
      cout << endl << s << " idx: " << i;
    }

    auto res = accumulate(data | views::drop(1), data[0],
                          [&](string s, string next)
                          {
                            return Reduce(std::string() + '[' + s + ',' + next + ']');
                          });

    auto sum = accumulate(res |
                            views::filter(
                              [](auto ch)
                              {
                                return ch != ',' && ch != '[' && ch != ']';
                              }) |
                            views::transform(
                              [](auto ch)
                              {
                                return ch - '0';
                              }),
                          0);
    return res;
    //    return to_string(res);
  }

  string Do2()
  {
    // string s    = data[0];
    // auto   node = ConstructArb(s);
    // Print(node, "init", 0);

    auto ziped = views::cartesian_product(data, data) | views::transform(
                                                          [&](auto a)
                                                          {
                                                            auto s1 = get<0>(a);
                                                            auto s2 = get<1>(a);
                                                            if (s1 == s2)
                                                              return 0;
                                                            auto node1 = ConstructArb(s1);
                                                            auto node2 = ConstructArb(s2);

                                                            auto add = Add(node1, node2);

                                                            Reduce(add);

                                                            return add->Sum();
                                                          });

    auto x = max(ziped);

    return to_string(x);
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample -1", "[.]")
{
  REQUIRE(Solve(1 + R"(
[[[[4,0],[5,4]],[[7,7],[6,0]]],[[8,[7,7]],[[7,9],[5,0]]]]
[[2,[[0,8],[3,4]]],[[[6,7],1],[7,[1,6]]]]
)")
            .Do() == "[[[[6,7],[6,7]],[[7,7],[0,7]]],[[[8,7],[7,7]],[[8,8],[8,0]]]]");
}

TEST_CASE(TODAY "Sample 0", "[.]")
{
  REQUIRE(Solve(1 + R"(
[[[[4,3],4],4],[7,[[8,4],9]]]
[1,1]
)")
            .Do() == "[[[[0,7],4],[[7,8],[6,0]]],[8,1]]");
}

TEST_CASE(TODAY "Sample 00", "[.]")
{
  REQUIRE(Solve(1 + R"(
[[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]]
[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]
[[2,[[0,8],[3,4]]],[[[6,7],1],[7,[1,6]]]]
[[[[2,4],7],[6,[0,5]]],[[[6,8],[2,8]],[[2,1],[4,5]]]]
[7,[5,[[3,8],[1,4]]]]
[[2,[2,2]],[8,[8,1]]]
[2,9]
[1,[[[9,3],9],[[9,0],[0,7]]]]
[[[5,[7,4]],7],1]
[[[[4,2],2],6],[8,7]]
)")
            .Do() == "[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]");
}

TEST_CASE(TODAY "Sample 1", "[.]")
{
  REQUIRE(Solve(1 + R"(
[[[0,[5,8]],[[1,7],[9,6]]],[[4,[1,2]],[[1,4],2]]]
[[[5,[2,8]],4],[5,[[9,9],0]]]
[6,[[[6,2],[5,6]],[[7,6],[4,7]]]]
[[[6,[0,7]],[0,9]],[4,[9,[9,0]]]]
[[[7,[6,4]],[3,[1,3]]],[[[5,5],1],9]]
[[6,[[7,3],[3,2]]],[[[3,8],[5,7]],4]]
[[[[5,4],[7,7]],8],[[8,3],8]]
[[9,3],[[9,9],[6,[4,9]]]]
[[2,[[7,7],7]],[[5,8],[[9,3],[0,2]]]]
[[[[5,2],5],[8,[3,7]]],[[5,[7,5]],[4,4]]]
)")
            .Do() == "[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]");
}

TEST_CASE(TODAY "Sample 2", "[x.]")
{
  REQUIRE(Solve(1 + R"(
[[[0,[5,8]],[[1,7],[9,6]]],[[4,[1,2]],[[1,4],2]]]
[[[5,[2,8]],4],[5,[[9,9],0]]]
[6,[[[6,2],[5,6]],[[7,6],[4,7]]]]
[[[6,[0,7]],[0,9]],[4,[9,[9,0]]]]
[[[7,[6,4]],[3,[1,3]]],[[[5,5],1],9]]
[[6,[[7,3],[3,2]]],[[[3,8],[5,7]],4]]
[[[[5,4],[7,7]],8],[[8,3],8]]
[[9,3],[[9,9],[6,[4,9]]]]
[[2,[[7,7],7]],[[5,8],[[9,3],[0,2]]]]
[[[[5,2],5],[8,[3,7]]],[[5,[7,5]],[4,4]]]
)")
            .Do2() == "3993");
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
