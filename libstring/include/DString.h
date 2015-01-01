
#ifndef DYABL_STRING_H__
#define DYABL_STRING_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "Utils.h"
#include <string.h>
/* BSD Functions */
#ifndef strlcpy
#define S_strlcpy strlcpy
#endif
#ifndef strlcat
#define S_strlcat strlcat
#endif

/* strdup */
#ifndef strdup
#define S_strdup strdup
#endif
char *S_strdup(const char *str);

/* strtok */
#ifndef strtok_r
#define S_strtok strtok
#define S_strtok_r strtok_r
#undef strtok
#undef strtok_r
#endif
char *S_strtok(char *__restrict str, const char *__restrict sep);
char *S_strtok_r(char *__restrict s, const char *__restrict delim,
                 char **__restrict last);

/* Up/Low */
#define S_strtoupper strtoupper
#define S_strtoupper_r strtoupper_r
#define S_strtolower strtolower
#define S_strtolower_r strtolower_r
void S_strtoupper(char *__restrict str);
void S_strtolower(char *__restrict str);
char *S_strtolower_r(const char *__restrict str);
char *S_strtoupper_r(const char *__restrict str);

/* Trim */
#define S_str_trim str_trim
#define S_strtrim strtrim
#define S_strrtrim strrtim
#define S_strltrim strltrim
#define S_strtrim_t strtrim_t
#define S_strrtrim_t strrtim_t
#define S_strltrim_t strltrim_t
char *S_str_trim(const char *str, const char *trim, int trim_mode);
char *S_strrtrim(const char *str);
char *S_strltrim(const char *str);
char *S_strtrim(const char *str);
char *S_strrtrim_t(const char *str, const char *trim);
char *S_strltrim_t(const char *str, const char *trim);
char *S_strtrim_t(const char *str, const char *trim);

/* Split */
#define S_strsplit strsplit
int S_strsplit(const char *str, const char *delim, char ***splited, size_t *nb);

/* Join */
#define S_strjoin strjoin
#define S_strjoin_a strjoin_a
char *S_strjoin(const char **array, size_t nb, const char *delim);
char *S_strjoin_a(const char **array, size_t nb, const char **array2,
                  size_t nb2);

#ifdef __cplusplus
}
#endif

#endif
