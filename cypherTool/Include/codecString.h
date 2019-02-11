/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file codecString.h
 * @author Marco "Swampy" Millan
 * @date 2019/02/10 2019
 * @brief Capsules all the coded tools in one class
 * 
 */
/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#pragma once
#include "cypherUtilities.h"

namespace toyboxSDK {

using std::string;
using std::map;
using std::multimap;
using std::vector;
using std::priority_queue;

class codecString
{
 public:
  /**
   * Default constructor
   */
  codecString() = default;
  
  /**
   * String defined constructor.
   * It defines the string and therefore the map.
   *
   * @param input
   *   The string to be sorted.
   */
  codecString(string input);

  /**
   * Default destructor
   */
  ~codecString() = default;
  
  /**
   * @brief Creates the map of how many times the letters repeat
   */
  void
  createMap();

 public:

  /**
   * Coded string
   */
  string m_string;

  /**
   * Map of how many times each letter repeats itself
   */
  map<char, int> m_dictionary;

  /**
   * Sorted chars by use from higher to lower
   */
  priority_queue< pair<char, int>, vector<pair<char, int>>, higherPair> m_sortedDictionary;
};

}