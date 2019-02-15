#pragma once

#include "tb_utils_prerequisites.h"

#include <fstream>

namespace toyboxSDK {

/**
* Class to use files.
*
* Sample usage:
*
* File sampleName;
* sampleName.open("fileToOpen.extension");
* sampleName.write(nByte, text);
* sampleName.close();
*/
class TB_UTIL_EXPORT File
{
 public:
  
  /**
  * Default class constructor.
  */
  File();

  /**
  * Default class destructor.
  */
  ~File();

  /**
  * Tries to open a file.
  * 
  * @param filename
  *  Name of the file to open.
  *
  * @return
  *   If file was successfuly open or not.
  */
  bool
  open(const TString filename);

  /**
  * Close the opened file.
  */
  void
  close();
  
  /**
  * Read the number of bytes in file and writes them in
  * a buffer. The read starts in the current position.
  *
  * @param nBytes
  *   Number of bytes to read on file.
  *
  * @param bytesOut
  *   Buffer where the bytes are going to be writed.
  */
  void
  read(const SizeT nBytes, ANSIChar* bytesOut);

  /**
  * TEST::Write
  *
  * Writes the given number of bytes in file.
  * Bytes to write are taken from the given buffer.
  * The writing starts in the current position.
  *
  * @param nBytes
  *   Number of bytes to write on file.
  *
  * @param bytesIn
  *   Buffer where the bytes to write are taken.
  */
  void
  write(const SizeT nBytes, const ANSIChar* bytesIn);

  /**
  * Return a TString of the current position + nBytes.
  *
  * @param nBytes
  *   Number of bytes to read as TString.
  *
  * @return
  *   Resulting TString.
  */
  TString
  getAsString(const SizeT nBytes);

  /**
  * Tells the size of file.
  *
  * @return
  *   Size in bytes of file.
  */
  SizeT
  size();

  /**
  * Moves the file pointer to given absolute position.
  *
  * @param position
  *   new file pointer position.
  */
  void
  seek(const SizeT position);

  /**
  * Tells the file pointer position.
  *
  * @return
  *   The pointer position.
  */
  SizeT
  tellG();

 protected:
  TString m_filename;
  SizeT m_size;
  std::fstream m_file;
};

}