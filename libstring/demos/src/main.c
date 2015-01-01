#include <DString.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void tok(void) {

  char blah[80], test[80];
  char *brkb, *brkt, *word;

  const char *sep = "\\/:;=-";
  const char *phrase;

  printf("String tokenizer test:\n");
  strcpy(test, "This;is.a:test:of=the/string\\tokenizer-function.");
  for (word = strtok(test, sep); word; word = strtok(NULL, sep))
    printf("Next word is \"%s\".\n", word);
  strcpy(test, "This;is.a:test:of=the/string\\tokenizer-function.");

  for (word = strtok_r(test, sep, &brkt); word;
       word = strtok_r(NULL, sep, &brkt)) {
    strcpy(blah, "blah:blat:blab:blag");

    for (phrase = strtok_r(blah, sep, &brkb); phrase;
         phrase = strtok_r(NULL, sep, &brkb))
      printf("So far we're at %s:%s\n", word, phrase);
  }
}

static void strFunc(void) {
  char *tmp = NULL;
  char *test = NULL;
  char *low = NULL;
  char *up = NULL;
  test = strdup("AzerTyqdsfSDGEfnlkdE");

  if (test == NULL)
    goto clean;

  free(tmp);
  tmp = strdup(test);
  if (tmp == NULL)
    goto clean;
  strtoupper(tmp);
  printf("%s : %s\n", test, tmp);

  free(tmp);
  tmp = strdup(test);
  if (tmp == NULL)
    goto clean;

  strtolower(tmp);
  printf("%s : %s\n", test, tmp);

  free(tmp);
  tmp = strdup(test);
  if (tmp == NULL)
    goto clean;
  low = strtolower_r(test);
  if (low == NULL)
    goto clean;
  up = strtoupper_r(test);
  if (up == NULL)
    goto clean;
  printf("%s : %s\n", test, low);
  printf("%s : %s\n", test, up);

clean:
  if (tmp)
    free(tmp);
  if (test)
    free(test);
  if (low)
    free(low);
  if (up)
    free(up);
}

static void trimTest(void) {
  const char *sample_strings[] = {"nothing to trim",
                                  "    trim the front",
                                  "trim the back     ",
                                  " trim one char front and back ",
                                  " trim one char front",
                                  "trim one char back ",
                                  "                   ",
                                  " ",
                                  "a",
                                  "",
                                  NULL};
  char test_buffer[64];
  int index;

  for (index = 0; sample_strings[index] != NULL; ++index) {
    memset(test_buffer, '\0', 64);
    strcpy(test_buffer, sample_strings[index]);
    printf("[%s] -> [%s]\n", sample_strings[index],
           str_trim(test_buffer, NULL, 3));
  }
  /* The test prints the following:
  [nothing to trim] -> [nothing to trim]
  [    trim the front] -> [trim the front]
  [trim the back     ] -> [trim the back]
  [ trim one char front and back ] -> [trim one char front and back]
  [ trim one char front] -> [trim one char front]
  [trim one char back ] -> [trim one char back]
  [                   ] -> []
  [ ] -> []
  [a] -> [a]
  [] -> []
  */
}

static void split(void) {

  char str[] = "JAN,FEB,MAR,APR,MAY,JUN,JULOCT,AUG,SEP,OCT,NOV,DEC,";

  char **res;
  size_t count = 0;

  int ret = strsplit(str, ",", &res, &count);
  printf("ret : %d count: %zu\n", ret, count);
  for (size_t k = 0; k < count; k++) {
    printf("str: %s\n", res[k]);
  }
  free(res);
}

static void join(void) {
  const char *str[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN",
                       "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

  char *s = strjoin(str, 12, "*");
  printf("%s %zu %c\n", s, strlen(s), s[strlen(s) - 1]);
  const char *delim[] = {"*", "- ", ","};
  free(s);
  s = strjoin_a(str, 12, delim, 3);
  printf("%s %zu %c\n", s, strlen(s), s[strlen(s) - 1]);
  free(s);
}

int main(void) {
  puts("Strtok_r");
  tok();
  puts("strlower, strupper");
  strFunc();
  puts("trim");
  trimTest();
  puts("split");
  split();
  puts("join");
  join();
  return EXIT_SUCCESS;
}
