#include "tb_file.h"

#include <iostream>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>

namespace toyboxSDK
{

File::File() : m_size(0) {
}

File::~File() {
}

bool
File::open(const TString filepath) {
  m_file.open(filepath, std::ios::in | std::ios::out | std::ios::binary);

  if (m_file) {
    m_filename = filepath;
    
    m_file.seekg(0, std::ios::end);

    m_size = static_cast<SizeT>(m_file.tellg());

    m_file.seekg(0, std::ios::beg);

    return true;
  }
  else {
    m_file.close();
    return false;
  }
}

void
File::close() {
  m_file.close();
  m_filename = TString();
  m_size = 0;
}

void
File::read(const SizeT nBytes, ANSIChar* bytesOut) {
  TB_ASSERT(tellG() + nBytes <= size());
  m_file.read(bytesOut, nBytes);
}

void
File::write(const SizeT nBytes, const ANSIChar* bytesIn) {
  m_file.write(bytesIn, nBytes);
}

TString
File::getAsString(const SizeT nBytes) {
  TB_ASSERT(tellG() + nBytes <= size());

  ANSIChar* tmpBuff = new ANSIChar[nBytes];
  read(nBytes, tmpBuff);

  TString resultingString;
  SizeT curIndex = 0;
  while (curIndex < nBytes) {
    resultingString += tmpBuff[curIndex];
    ++curIndex;
  }

  delete[] tmpBuff;

  return resultingString;
}

SizeT
File::size() {
  return m_size;
}

void
File::seek(const SizeT position) {
  TB_ASSERT(position <= size());
  m_file.seekg(static_cast<std::streampos>(position));
}

SizeT
File::tellG() {
  return static_cast<SizeT>(m_file.tellg());
}

}