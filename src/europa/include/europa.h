#define EU_VERSION "0.1"
#define EU_BUILT_DATE __TIMESTAMP__

char europa_shell_mode;

#if DEBUG
#define EU_DEBUG_INFO "with debug flag"
#else
#define EU_DEBUG_INFO ""
#endif

// Detect where the program is being compiled... 
// 
#ifdef _WIN32
    // Windows (x86)
    #define EU_BUILT_OS "Windows"
#elif _WIN64
    // Windows (x64)
    #define EU_BUILT_OS "Windows x64"
#elif __unix__ 
    // Unix
    #define EU_BUILT_OS "Windows"
#elif __linux__
    // linux
    #define EU_BUILT_OS "Linux"
#elif __APPLE__
    // Mac OS, not sure if this is covered by __posix__ and/or __unix__ though...
    #define EU_BUILT_OS "Mac"
#else 
    #define EU_BUILT_OS "Unkown"
#endif