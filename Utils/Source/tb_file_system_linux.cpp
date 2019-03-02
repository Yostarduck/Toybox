#include "tb_file_system.h"
#if TB_PLATFORM == TB_PLATFORM_LINUX
#include <unistd.h>

namespace toyboxSDK
{

TString
FileSystem::GetWorkingPath() {
  TString fullpath;
  ANSIChar cCurrentPath[FILENAME_MAX];

  if (!getcwd(cCurrentPath, sizeof(cCurrentPath))) {
    return fullpath;
  }

  cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';

  SizeT curChar = 0;
  while (curChar < sizeof(cCurrentPath)) {
    fullpath += cCurrentPath[curChar];
    ++curChar;
  }

  return fullpath;
}

}

#endif