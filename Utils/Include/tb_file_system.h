#pragma once
#include "tb_utils_prerequisites.h"
#include "tb_file.h"
#include <vector>

namespace toyboxSDK {

/**
* Class to load files in buffers
*
* Sample usage:
* FileSystem sampleName;
*/
class TB_UTILS_EXPORT FileSystem
{
 public:
  
  /**
  * Default FileSystem constructor
  */
  FileSystem();

  /**
  * Default FileSystem constructor
  */
  ~FileSystem();
  
  /**
  * Returns the fullpath of the working directory.
  * OS specific.
  *
  * @return
  *   TString containing the fullpath of the working directory.
  */
  TString
  GetWorkingPath();

  
  /**
  * Returns the name of the app (includes extension).
  * OS specific.
  *
  * @return
  *   TString containing the name of the app.
  */
  TString
  GetAppName();

  /**
  * Returns the fullpath of the working directory.
  * OS specific.
  *
  * @return
  *   TString containing the fullpath of the working directory.
  */
  std::vector<TString>
  GetDirectoryContent(TString path = _T(""));

  /**
  * Creates a File and open it.
  *
  * @param filename
  *  Name of the new file to create.
  *
  * @param file
  *  File class whichs will handle te opened file.
  *
  * @return
  *  true if file is successfully created.
  */
  bool
  CreateAndOpen(const TString& filename, File& file);

  /**
  * TEST::Copy
  *
  * Copies a File.
  *
  * @param filepathSrc
  *  Path of the source file to copy.
  *
  * @param filepathDst
  *  Path of the destiny file.
  */
  void
  Copy(const TString& filepathSrc, const TString& filepathDst);

  /**
  * TEST::Move
  *
  * Moves a file.
  *
  * @param filepathSrc
  *  Path of the file to move.
  *
  * @param filepathDst
  *  Destiny path.
  */
  void
  Move(const TString& filepathSrc, const TString& filepathDst);

  /**
  * TEST::Remove
  *
  * Removes a file.
  *
  * @param filepath
  *  Path of file to remove.
  *
  * @return
  *  True if the file was successfully removed.
  */
  bool
  Remove(const TString& filepath);

  /**
  * TEST::IsFile
  *
  * Checks is given path is a file.
  *
  * @param filepath
  *
  * @return
  *  True if is file.
  */
  bool
  IsFile(const TString& filepath);

  /**
  * TEST::IsDirectory
  *
  * Checks is given path is a directory.
  *
  * @param filepath
  *
  * @return
  *  True if is directory.
  */
  bool
  IsDirectory(const TString& filepath);

  /**
  * TEST::Exists
  *
  * Checks if path exists.
  *
  * @param filepath
  *
  * @return
  *  True if exists.
  */
  bool
  Exists(const TString& filepath);

  /**
  * TEST::GetFileExtension
  *
  * Returns the extension of a file.
  *
  * @param filepath
  *  Path where the file extension will be taken.
  *
  * @return
  *  The file extension.
  */
  static TString 
  GetFileExtension(const TString& filepath);

  static TString
  GetFileName(const TString& filepath);

  static void
  ScanDir(String paht);

};

}