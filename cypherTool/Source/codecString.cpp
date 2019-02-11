#include "codecString.h"

namespace toyboxSDK {

codecString::codecString(WString input) {
  m_string = input;

  //Makes the whole string to lowercase.
  std::transform(m_string.begin(),
                 m_string.end(),
                 m_string.begin(),
                 ::tolower);

  //Removes spaces from string
  m_string.erase(std::remove(m_string.begin(), m_string.end(), ' '),
                 m_string.end());

  createMap();
}


void
codecString::createMap() {
  WString alphabet = _T("abcdefghijklmn@opqrstuvwxyz"); //esp
  //String alphabet = "etaoinshrdlcumwfgypbvkjxqz"; //eng
  
  m_dictionary.clear();
  for (WChar& c : alphabet) {
    m_dictionary.insert(std::make_pair(c, 0));
  }

  for (WChar& c : m_string) {
    m_dictionary[c] += 1;
  }

  for (auto it = m_dictionary.begin(); it != m_dictionary.end(); ++it) {
    m_sortedDictionary.push(std::make_pair(it->first, it->second));
  }
}

}