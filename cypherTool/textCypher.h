/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file textCypher.h
 * @author Marco "Swampy" Millan
 * @date 2019/02/10 2019
 * @brief is a Coder-Decoder library with several CODEC functions
 * 
 */
/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#pragma once

#include "cypherUtilities.h"

#include <iostream>
#include <ctype.h>

namespace toyboxSDK {

using std::vector;
using std::map;
using std::cout;
using std::endl;

class textCypher
{
public:
  /**
   * Default constructor
   */
  textCypher() {
    //initializes with a default alphabet
    vector<WChar> latin;
    WString defaultAlphabet = _T("abcdefghijklmnopqrstuvwxyz");
    for (WChar& c : defaultAlphabet) {
      latin.push_back(c);
    }
    m_alphabetMap.insert(std::make_pair(_T("latin"), latin));

  }

  /**
   * Default destructor
   */
  ~textCypher() = default;

  static WString caesarDecrypt(WString input, Int32 offset, WString alphabet) {
    //Variables
    vector<WChar> shifted;
    vector<WChar> original;
    WString output = _T("");
    
    auto it = m_alphabetMap.find(alphabet);
    
    //Checks if the alphabet actually exists
    if ( it == m_alphabetMap.end()) {
      //Doest return a shifted text
      cout << "Alphabet not mapped" << endl;
      return output;
    }
    original = it->second;

    //Shifts the alphabet by offset in a new Vector called shifted
    for (UInt32 i = 0; i < original.size(); ++i) {
      int shift = i + offset;
      if (i + offset == original.size()) {
        shift -= original.size();
      }
      if (i + offset < 0) {
        shift += original.size();
      }
      shifted[i] = original[i + offset];
    }

    //Decrypts the text

    for (WChar& c : input) {
      if (isalpha(c)) {

      }
      output += c;
    }

    return output;
  }

  static map<WString, vector<WChar>> m_alphabetMap;

};

}