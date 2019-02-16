/*||���||���||���||���||���||���||���||���||���||���||���||���||���||���||���||*/
/**
 * @file cypherUtilities.h
 * @author Marco "Swampy" Millan
 * @date 2019/02/10 2019
 * @brief All utilities are here
 * 
 */
/*||���||���||���||���||���||���||���||���||���||���||���||���||���||���||���||*/
#pragma once

#include <tb_types.h>

#include <algorithm>
#include <map>
#include <queue>
#include <string>
#include <vector>

namespace toyboxSDK {

using std::pair;
using std::multimap;

struct higherPair : public std::binary_function<pair<WChar, Int32>,
                                                pair<WChar, Int32>,
                                                bool>
{
  bool
  operator()(const pair<WChar, Int32> left, 
             const pair<WChar, Int32> right) const {
    return left.second < right.second;
  }
};

template<typename A, typename B>
std::pair<B, A>
flip_pair(const std::pair<A, B> &p) {
  return std::pair<B, A>(p.second, p.first);
}

template<typename A, typename B>
multimap<B, A>
flip_map(const std::map<A, B> &src) {
  multimap<B, A> dst;
  std::transform(src.begin(),
                 src.end(),
                 std::inserter(dst, dst.begin()),
                 flip_pair<A, B>);
  return dst;
}

}