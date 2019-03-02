#include "tb_file_system.h"
#include "tb_string_conversions.h"

namespace toyboxSDK
{

FileSystem::FileSystem() {
}

FileSystem::~FileSystem() {
}

bool
FileSystem::CreateAndOpen(const TString& filename, File& file) {
  std::ofstream newFile(filename);
  newFile.close();

  return file.open(filename);
}

void
FileSystem::Copy(const TString& filepathSrc, const TString& filepathDst) {
  File srcFile;
  srcFile.open(filepathSrc);

  File dstFile;
  CreateAndOpen(filepathDst, dstFile);

  SizeT srcSz = srcFile.size();
  ANSIChar* srcFileBytes = new ANSIChar[srcSz];
  srcFile.read(srcSz, &srcFileBytes[0]);
  srcFile.close();

  dstFile.write(srcSz, &srcFileBytes[0]);
  dstFile.close();

  delete[] srcFileBytes;
}

void
FileSystem::Move(const TString& filepathSrc, const TString& filepathDst) {
  Copy(filepathSrc, filepathDst);
  Remove(filepathSrc);
}

bool
FileSystem::Remove(const TString& filepath) {
  bool successfullyErased = remove(StringConversion::toString(filepath).c_str()) == 0;

  return successfullyErased;
}

bool
FileSystem::IsFile(const TString& filepath) {
  struct stat s;

  if (stat(StringConversion::toString(filepath).c_str(), &s) == 0) {
    if (s.st_mode & S_IFREG) {
      return true;
    }
  }

  return false;
}

bool
FileSystem::IsDirectory(const TString& filepath) {
  TString rename = filepath;

  struct stat s;

  if (stat(StringConversion::toString(rename).c_str(), &s) == 0) {
    if (s.st_mode & S_IFDIR) {
      return true;
    }
  }

  return false;
}

bool
FileSystem::Exists(const TString& filepath) {
  std::ifstream f(filepath.c_str());
  return f.good();
}

TString
FileSystem::GetFileExtension(const TString& file) {
  SizeT pos = file.find_last_of(_T('.'));

  TString extension;

  if (pos != TString::npos) {
    extension = file.substr(pos + 1);
  }

  return extension;
}

TString
FileSystem::GetFileName(const TString& filepath) {
  SizeT slashPos = filepath.find_last_of(_T('\\'));
  SizeT dotPos = filepath.find_last_of(_T('.'));

  TString name;

  if (slashPos != TString::npos) {
    name = filepath.substr(slashPos + 1, dotPos - slashPos - 1);
  } 
  else {
    name = filepath.substr(0, dotPos);
  }

  return name;
}

}