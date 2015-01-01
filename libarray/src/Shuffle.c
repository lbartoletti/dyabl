#include "Array.h"
#include "Rand.h"
#include "Utils.h"
#include <string.h>
#include <time.h>
#if defined(__unix__) || defined(__APPLE__)
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#endif

bool Shuffle(void *tab, size_t nb_elem, size_t size) {
  unsigned long long seed;
#ifdef __WIN32__
  seed = GetCurrentProcessId();
#elif defined(__unix__) || defined(__APPLE__)
  FILE *fp = NULL;

  if ((fp = fopen("/dev/urandom", "r")) == NULL) {
    int pid = getpid();
    struct timeval time;
    gettimeofday(&time, NULL);
    seed = (unsigned long long)(time.tv_sec * time.tv_usec * pid);
  } else {
    if (fread(&seed, sizeof(seed), 1, fp) != sizeof(seed)) {
      seed = (unsigned long long)(time(NULL));
    }
  }
#else
  seed = time(NULL);
#endif

  size_t length = nb_elem;
  Lcg_s minstd_rand = {seed, 48271, 0, 2147483647};

  void *copy = NULL;
  if ((copy = malloc(nb_elem * size)) == NULL) {

#if defined(__unix__) || defined(__APPLE__)
    if (fp) {
      fclose(fp);
    }
#endif
    return false;
  }

  size_t i = 0;
  while (i <= length) {
    size_t s = lcg_st(&minstd_rand) % (length - 1);
    memcpy((char *)copy + i * size, (char *)tab + s * size, size);
    genswap((char *)tab + s * size, (char *)tab + (nb_elem - 1) * size, size);
    i++;
    length--;
  }

#if defined(__unix__) || defined(__APPLE__)
  if (fp) {
    fclose(fp);
  }
#endif
  free(copy);
  return true;
}
