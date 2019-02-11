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
  codecString(WString input);

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
  WString m_string;

  /**
   * Map of how many times each letter repeats itself
   */
  map<WChar, Int32> m_dictionary;

  /**
   * Sorted chars by use from higher to lower
   */
  priority_queue<pair<WChar, Int32>, 
                 vector<pair<WChar, Int32>>, 
                 higherPair> m_sortedDictionary;
};

}