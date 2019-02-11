/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file main.cpp
 * @author Marco "Swampy" Millan
 * @date 2019/02/10 2019
 * @brief main of decoder project
 * 
 */
/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#include "letterMap.h"
#include "codecString.h"

#include <vector>
#include <iostream>

int
main(int argc, char* argv[]) {
  std::string sSpanishFrequency = "EAOSRNIDLCTUMPBGVYQHFZJ0XKW";
  std::transform(sSpanishFrequency.begin(),
                 sSpanishFrequency.end(),
                 sSpanishFrequency.begin(),
                 ::tolower);

  std::string secretText = "BUEFR FGS PSKEKBSEN CGEIMC LWBSTGXJN RRIIOXF AJO JIYFPEENA NVRII\
                            SLUHW BDD TZY ZSMIPMQUB E YCMYGT CI VGLGQES D CFGSSPJ EDMKCBMYJMG\
                            ZRHX XDD QXKU LUQ CVHTZWCCYN WQJ VDPZVVOSJZO THZK PP EWZP CKGKCZ\
                            ZHQ NUBOCLAALSGV GCILFBNN WZS AKF RPXME YVN EIAQHBD F CBAASD";
  std::transform(secretText.begin(),
                 secretText.end(),
                 secretText.begin(),
                 ::tolower);

  std::vector<char> vecSpanishFrequency;
  for (auto c : sSpanishFrequency) {
    vecSpanishFrequency.push_back(c);
  }

  toyboxSDK::codecString spanishCodec(secretText);

  std::vector<char> result;
  while (!spanishCodec.m_sortedDictionary.empty()) {
    result.push_back(spanishCodec.m_sortedDictionary.top().first);
    spanishCodec.m_sortedDictionary.pop();
  }

  std::cout << "Spanish freq\tText free" << std::endl;

  int lettersSz = result.size();
  std::map<char, char> replacer;
  for (int i = 0; i < lettersSz; ++i) {
    replacer.insert(std::make_pair(result[i], vecSpanishFrequency[i]));

    std::cout << vecSpanishFrequency[i] << "\t\t" << result[i] << std::endl;
  }
  replacer.insert(std::make_pair(' ', ' '));

  std::cout << std::endl;
  for (auto it : secretText) {
    std::cout << replacer[it];
  }


  return 0;
}