
#ifndef I_E_STD_H 
#define I_E_STD_H 

#define SYM_MAX_SIZE 32
#define USER_FUNCTION_NAME_MAX_SIZE 32
#define VAR_NAME_MAX_SIZE 32

#ifdef _WIN32
#define _STRING_DUP _strdup
#else
#define _STRING_DUP strdup
#endif

#endif