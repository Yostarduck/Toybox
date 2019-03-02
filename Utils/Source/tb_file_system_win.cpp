#include "tb_file_system.h"

#if TB_PLATFORM == TB_PLATFORM_WINDOWS
#include <direct.h>
#include <tb_string_conversions.h>
#include <windows.h>

namespace toyboxSDK
{

TString
FileSystem::GetWorkingPath() {
  String fullpath;
  ANSIChar cCurrentPath[FILENAME_MAX];

  GetModuleFileNameA(NULL, cCurrentPath, MAX_PATH);

  fullpath.append(cCurrentPath, sizeof(cCurrentPath) - 1);

  SizeT backslash = fullpath.find_last_of('\\');
  TB_ASSERT(String::npos != backslash);
  fullpath.erase(fullpath.begin() + backslash, fullpath.end());
  fullpath.append("\\");

  return StringConversion::toTString(fullpath);
}

TString
FileSystem::GetAppName() {
  String fullpath;
  ANSIChar cCurrentPath[FILENAME_MAX];

  GetModuleFileNameA(NULL, cCurrentPath, MAX_PATH);

  fullpath.append(cCurrentPath, sizeof(cCurrentPath) - 1);

  SizeT backslash = fullpath.find_last_of('\\');
  TB_ASSERT(String::npos != backslash);
  fullpath.erase(fullpath.begin(), fullpath.begin() + backslash + 1);

  return StringConversion::toTString(fullpath);

}

std::vector<TString>
FileSystem::GetDirectoryContent(TString path) {
  std::vector<TString> content;

  if (path.empty()) {
    path = GetWorkingPath();
  }

  if (path.back() != '\\') {
    path += _T('\\');
  }
  path += _T("*.*");

  WIN32_FIND_DATA ffd;

  HANDLE hFind = FindFirstFile(StringConversion::toWString(path).c_str(), &ffd);
  if (INVALID_HANDLE_VALUE == hFind) {
    //Logger::addError(__FILE__, __LINE__, _T("Invalid Path. ") + GetLastError());
    return content;
  }

  bool stillContent = true;
  while (stillContent) {
    content.push_back(ffd.cFileName);

    stillContent = FindNextFile(hFind, &ffd);
  }

  return content;
}

}

#endif