#ifndef I_LINUX_API_H
#define I_LINUX_API_H

// Linux specific APIs should be implemented here 

#include <unistd.h>
#define EU_GET_CWD(BUFFER, SIZE) getcwd(BUFFER, SIZE)

#endif