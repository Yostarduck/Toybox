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

  /**
   * @brief Basically Caesar but with a specific key
   * @param WString input, WChar key for the first letter of the offset, WString alphabet
   * @return the decoded string
   *
   */
  static WString vignereDecrypt(WString input, WChar first, WString alphabet) {
    UInt32 i = 0;
    vector<WChar> original;
    WString output = _T("");

    auto it = m_alphabetMap.find(alphabet);

    //Checks if the alphabet actually exists
    if (it == m_alphabetMap.end()) {
      //Doest return a shifted text
      cout << "Alphabet not mapped" << endl;
      return output;
    }
    original = it->second;

    //Gets the int corresponding to the char
    for (i = 0; i < original.size(); ++i) {
      if (first == original[i]) {
        return caesarDecrypt(input, i, alphabet);
      }
    }

  }

  /**
   * @brief codex based in an offset
   * @param WString input, offset for the first letter of the offset, WString alphabet
   * @return the decoded string
   *
   */
  static WString caesarDecrypt(WString input, Int32 offset, WString alphabet) {
    //Variables
    map<WChar, WChar> shifted;
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
      shifted.insert(std::make_pair(original[i], original[shift]));
    }

    //Decrypts the text
    for (WChar c : input) {
      if (isalpha(c)) {
        c = shifted[c];
      }
      output += c;
    }

    return output;
  }

  static map<WString, vector<WChar>> m_alphabetMap;

};

}