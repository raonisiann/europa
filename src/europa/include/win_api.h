#ifndef E_WIN_API_H
#define E_WIN_API_H

// Windows specific functions should be mapped here 

// mapping for _getcwd (getcwd on linux)
// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/getcwd-wgetcwd?view=vs-2019
#include <direct.h>
#define EU_GET_CWD(BUFFER, SIZE) _getcwd(BUFFER, SIZE)

#endif 