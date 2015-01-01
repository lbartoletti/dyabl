#include <Q32_32.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  if (argc != 3) {
    printf("Usage: %s number number\n", argv[0]);
    return 1;
  }

  Q32_32 a, b;
  a = Q32_32_from_int((int32_t)strtol(argv[1], NULL, 10));
  b = Q32_32_from_int((int32_t)strtol(argv[2], NULL, 10));

  bool over = false;
  Q32_32 prod = Q32_32_mul_noof(a, b, &over);
  printf("%d %d\n", over, Q32_32_to_int(prod));

  printf("%lld\n", Q32_32_from_int(1));
  return 0;
}
