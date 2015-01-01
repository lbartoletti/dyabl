#include <DString.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

/* STRLCPY FROM OPENBSD */

/*
 * Copyright (c) 1998, 2015 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * Copy string src to buffer dst of size dsize.  At most dsize-1
 * chars will be copied.  Always NUL terminates (unless dsize == 0).
 * Returns strlen(src); if retval >= dsize, truncation occurred.
 */
size_t S_strlcpy(char *dst, const char *src, size_t dsize) {
  const char *osrc = src;
  size_t nleft = dsize;

  /* Copy as many bytes as will fit. */
  if (nleft != 0) {
    while (--nleft != 0) {
      if ((*dst++ = *src++) == '\0') {
        break;
      }
    }
  }

  /* Not enough room in dst, add NUL and traverse rest of src. */
  if (nleft == 0) {
    if (dsize != 0) {
      *dst = '\0'; /* NUL-terminate dst */
    }
    while (*src++) {
      ;
    }
  }

  return (size_t)(src - osrc - 1); /* count does not include NUL */
}

/* STRLCAT FROM OPENBSD */

/*
 * Copyright (c) 1998, 2015 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * Appends src to string dst of size dsize (unlike strncat, dsize is the
 * full size of dst, not space left).  At most dsize-1 characters
 * will be copied.  Always NUL terminates (unless dsize <= strlen(dst)).
 * Returns strlen(src) + MIN(dsize, strlen(initial dst)).
 * If retval >= dsize, truncation occurred.
 */
size_t S_strlcat(char *dst, const char *src, size_t dsize) {
  const char *odst = dst;
  const char *osrc = src;
  size_t n = dsize;
  size_t dlen;

  /* Find the end of dst and adjust bytes left but don't go past end. */
  while (n-- != 0 && *dst != '\0') {
    dst++;
  }
  dlen = (size_t)(dst - odst);
  n = dsize - dlen;

  if (n-- == 0) {
    return (dlen + strlen(src));
  }
  while (*src != '\0') {
    if (n != 0) {
      *dst++ = *src;
      n--;
    }
    src++;
  }
  *dst = '\0';

  return (size_t)(dlen + (size_t)(src - osrc)); /* count does not include NUL */
}

char *S_strdup(const char *str) {
  size_t siz;
  char *copy;

  siz = strlen(str) + 1;
  if ((copy = malloc(siz)) == NULL) {
    return (NULL);
  }
  (void)memcpy(copy, str, siz);
  return (copy);
}

char *S_strtok(char *restrict str, const char *restrict sep) {
  static char *last;

  return strtok_r(str, sep, &last);
}

char *S_strtok_r(char *restrict s, const char *restrict delim,
                 char **restrict last) {
  const char *spanp;
  int c, sc;
  char *tok;

  if (s == NULL && (s = *last) == NULL) {
    return (NULL);
  }

  /*
   * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
   */
cont:
  c = *s++;
  for (spanp = delim; (sc = *spanp++) != 0;) {
    if (c == sc) {
      goto cont;
    }
  }

  if (c == 0) { /* no non-delimiter characters */
    *last = NULL;
    return (NULL);
  }
  tok = s - 1;

  /*
   * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
   * Note that delim must have one NUL; we stop if we see that, too.
   */
  for (;;) {
    c = *s++;
    spanp = delim;
    do {
      if ((sc = *spanp++) == c) {
        if (c == 0) {
          s = NULL;
        } else {
          s[-1] = '\0';
        }
        *last = s;
        return (tok);
      }
    } while (sc != 0);
  }
  /* NOTREACHED */
}

static void S_strtofunc(char *restrict str, int (*func)(int)) {
  while (*str) {
    *str = (char)func((int)*str);
    str++;
  }
}

void S_strtoupper(char *restrict str) { S_strtofunc(str, toupper); }

char *S_strtoupper_r(const char *restrict str) {
  char *s = NULL;

  if (str != NULL) {

    s = malloc(sizeof(*s) * (strlen(str) + 1));
    if (s != NULL) {
      int i;
      for (i = 0; str[i]; i++) {
        s[i] = (char)toupper((int)str[i]);
      }
      s[i] = '\0';
    }
  }
  return s;
}

void S_strtolower(char *restrict str) { S_strtofunc(str, tolower); }

char *S_strtolower_r(const char *restrict str) {
  char *s = NULL;

  if (str != NULL) {

    s = malloc(sizeof(*s) * (strlen(str) + 1));
    if (s != NULL) {
      int i;
      for (i = 0; str[i]; i++) {
        s[i] = (char)tolower((int)str[i]);
      }
      s[i] = '\0';
    }
  }
  return s;
}

char *S_str_trim(const char *restrict str, const char *restrict trim,
                 int trim_mode) {
  if (trim_mode < 1 || trim_mode > 3) {
    return DECONST(char *, str);
  }

  int defaultTrim = false;
  if (trim == NULL) {
    defaultTrim = true;
  }

  char *left = DECONST(char *, str);
  char *right; // = DECONST (char*, str) + strlen (str);

  if (defaultTrim) {
    if (trim_mode & 1) {
      while (*left && isspace(*left)) {
        left++;
      }
    }
    right = left + (strlen(left) - 1);
    if (trim_mode & 2) {
      while (isspace(*right) && (right - left) >= 0) {
        right--;
      }
    }
  } else {
    if (trim_mode & 1) {
      while (*left && strchr(trim, *left)) {
        left++;
      }
    }
    right = left + (strlen(left) - 1);
    if (trim_mode & 2) {
      while (strchr(trim, *right) != NULL && (right - left) >= 0) {
        right--;
      }
    }
  }

  size_t len = (size_t)(right - left) + 1;

  // TODO: it's ugly
  char *s = NULL;
  if (len > 0) {
    s = malloc(len);
    if (s) {
      memcpy(s, left, len);
      s[len] = '\0';
    }
  } else {
    return S_strdup("");
  }

  return s;
}

char *S_strltrim(const char *str) { return S_str_trim(str, NULL, 1); }
char *S_strrtrim(const char *str) { return S_str_trim(str, NULL, 2); }
char *S_strtrim(const char *str) { return S_str_trim(str, NULL, 3); }
char *S_strltrim_t(const char *str, const char *trim) {
  return S_str_trim(str, trim, 1);
}
char *S_strrtrim_t(const char *str, const char *trim) {
  return S_str_trim(str, trim, 2);
}
char *S_strtrim_t(const char *str, const char *trim) {
  return S_str_trim(str, trim, 3);
}

int S_strsplit(const char *str, const char *delim, char ***splited,
               size_t *nb) {
  *nb = 0;
  *splited = NULL;

  if (!delim) {
    return -1;
  }

  size_t count = 0;
  char *save;
  char *tok;
  char **tab = NULL;
  char *s = strdup(str);
  if (s) {
    for (tok = strtok_r(s, delim, &save); tok;
         tok = strtok_r(NULL, delim, &save)) {
      char **tmp = realloc(tab, (count + 1) * sizeof(*tab));
      if (tmp) {
        tab = tmp;
        *(tab + count) = strdup(tok);
        count++;
      } else {
        free(tmp);
        free(tab);
        free(s);
        return -2;
      }
    }

    *splited = tab;
    *nb = count;
  }
  free(s);

  return 1;
}

char *S_strjoin(const char **array, const size_t nb, const char *delim) {
  size_t i = 1;
  size_t lenArray = 1;
  lenArray += strlen(array[0]);
  size_t lenDim = strlen(delim);

  while (i < nb) {
    lenArray += lenDim;
    lenArray += strlen(array[i]);
    i++;
  }

  char *str = NULL;
  str = malloc((lenArray) * sizeof(char));
  if (str != NULL) {
    strlcpy(str, array[0], lenArray * sizeof(char));
    for (i = 1; i < nb; i++) {
      strlcat(str, delim, lenArray * sizeof(char *));
      strlcat(str, array[i], lenArray * sizeof(char));
    }

    return str;
  }

  return NULL;
}

char *S_strjoin_a(const char **array1, const size_t nb, const char **array2,
                  const size_t nb2) {
  size_t i = 1;
  size_t lenArray = 1;
  lenArray += strlen(array1[0]);

  while (i < nb) {
    lenArray += strlen(array2[i % nb2]);
    lenArray += strlen(array1[i]);
    i++;
  }

  char *str = NULL;
  str = malloc(lenArray);
  if (str != NULL) {
    strlcpy(str, array1[0], lenArray * sizeof(char));
    for (i = 1; i < nb; i++) {
      strlcat(str, array2[(i - 1) % nb2], lenArray * sizeof(char));
      strlcat(str, array1[i], lenArray * sizeof(char));
    }

    return str;
  }

  return NULL;
}
