/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file letterMap.h
 * @author Marco "Swampy" Millan
 * @date 2019/02/10 2019
 * @brief Letter mapped by frequency in X language
 * 
 */
/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#pragma once
#include "cypherUtilities.h"

namespace toyboxSDK {

using std::map;
using std::pair;
using std::priority_queue;
using std::vector;

class letterMap
{
 public:
  
  /**
   * Default constructor
   */
  letterMap() = default;

  /**
   * Default destructor
   */
  ~letterMap() = default;

  /**
   * @brief Loads the map with the letters by use in the language
   * @param 
   * @return 
   *
   */
  void
  loadMap();

 public:
  
  /**
   * Map of letters with amount of use
   */
  map<WChar, Int32> m_dictionary;

  /**
   * Sorted chars by use
   */
  priority_queue<       pair<WChar, Int32>, 
                 vector<pair<WChar, Int32>>, 
                 higherPair>                  m_sortedDictionary;
};

}