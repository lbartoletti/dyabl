extern "C" {
#include "DString.h"
}

#include <gtest/gtest.h>

TEST(STRING, trim) {
  const char *sample_strings[] = {"nothing to trim",                // 0
                                  "    trim the front",             // 1
                                  "trim the back     ",             // 2
                                  " trim one char front and back ", // 3
                                  " trim one char front",           // 4
                                  "trim one char back ",            // 5
                                  "                   ",            // 6
                                  " ",                              // 7
                                  "a",                              // 8
                                  "",                               // 9
                                  "****trim////",                   // 10
                                  NULL};

  ASSERT_STREQ(sample_strings[0], str_trim(sample_strings[0], NULL, 0));
  ASSERT_STREQ(sample_strings[0], str_trim(sample_strings[0], NULL, 4));

  ASSERT_STREQ("nothing to trim", str_trim(sample_strings[0], NULL, 1));
  ASSERT_STREQ("nothing to trim", S_strrtrim(sample_strings[0]));
  ASSERT_STREQ("nothing to trim", str_trim(sample_strings[0], NULL, 2));
  ASSERT_STREQ("nothing to trim", S_strltrim(sample_strings[0]));
  ASSERT_STREQ("nothing to trim", str_trim(sample_strings[0], NULL, 3));
  ASSERT_STREQ("nothing to trim", S_strtrim(sample_strings[0]));

  ASSERT_STREQ("trim the front", str_trim(sample_strings[1], NULL, 1));
  ASSERT_STREQ("trim the front", S_strltrim(sample_strings[1]));
  ASSERT_STREQ(sample_strings[1], str_trim(sample_strings[1], NULL, 2));
  ASSERT_STREQ(sample_strings[1], S_strrtrim(sample_strings[1]));
  ASSERT_STREQ("trim the front", str_trim(sample_strings[1], NULL, 3));
  ASSERT_STREQ("trim the front", S_strtrim(sample_strings[1]));

  ASSERT_STREQ("trim one char front", str_trim(sample_strings[4], NULL, 1));
  ASSERT_STREQ("trim one char front", S_strltrim(sample_strings[4]));
  ASSERT_STREQ(sample_strings[4], str_trim(sample_strings[4], NULL, 2));
  ASSERT_STREQ(sample_strings[4], S_strrtrim(sample_strings[4]));
  ASSERT_STREQ("trim one char front", str_trim(sample_strings[4], NULL, 3));
  ASSERT_STREQ("trim one char front", S_strtrim(sample_strings[4]));

  ASSERT_STREQ(sample_strings[2], str_trim(sample_strings[2], NULL, 1));
  ASSERT_STREQ(sample_strings[2], S_strltrim(sample_strings[2]));
  ASSERT_STREQ("trim the back", str_trim(sample_strings[2], NULL, 2));
  ASSERT_STREQ("trim the back", S_strrtrim(sample_strings[2]));
  ASSERT_STREQ("trim the back", str_trim(sample_strings[2], NULL, 3));
  ASSERT_STREQ("trim the back", S_strtrim(sample_strings[2]));

  ASSERT_STREQ(sample_strings[5], str_trim(sample_strings[5], NULL, 1));
  ASSERT_STREQ(sample_strings[5], S_strltrim(sample_strings[5]));
  ASSERT_STREQ("trim one char back", str_trim(sample_strings[5], NULL, 2));
  ASSERT_STREQ("trim one char back", S_strrtrim(sample_strings[5]));
  ASSERT_STREQ("trim one char back", str_trim(sample_strings[5], NULL, 3));
  ASSERT_STREQ("trim one char back", S_strtrim(sample_strings[5]));

  ASSERT_STREQ("trim one char front and back ",
               str_trim(sample_strings[3], NULL, 1));
  ASSERT_STREQ("trim one char front and back ", S_strltrim(sample_strings[3]));
  ASSERT_STREQ(" trim one char front and back",
               str_trim(sample_strings[3], NULL, 2));
  ASSERT_STREQ(" trim one char front and back", S_strrtrim(sample_strings[3]));
  ASSERT_STREQ("trim one char front and back",
               str_trim(sample_strings[3], NULL, 3));
  ASSERT_STREQ("trim one char front and back", S_strtrim(sample_strings[3]));

  ASSERT_STREQ("", str_trim(sample_strings[6], NULL, 1));
  ASSERT_STREQ("", S_strltrim(sample_strings[6]));
  ASSERT_STREQ("", str_trim(sample_strings[6], NULL, 2));
  ASSERT_STREQ("", S_strrtrim(sample_strings[6]));
  ASSERT_STREQ("", str_trim(sample_strings[6], NULL, 3));
  ASSERT_STREQ("", S_strtrim(sample_strings[6]));

  ASSERT_STREQ("", str_trim(sample_strings[7], NULL, 1));
  ASSERT_STREQ("", S_strltrim(sample_strings[7]));
  ASSERT_STREQ("", str_trim(sample_strings[7], NULL, 2));
  ASSERT_STREQ("", S_strrtrim(sample_strings[7]));
  ASSERT_STREQ("", str_trim(sample_strings[7], NULL, 3));
  ASSERT_STREQ("", S_strtrim(sample_strings[7]));

  ASSERT_STREQ(sample_strings[8], str_trim(sample_strings[8], NULL, 1));
  ASSERT_STREQ(sample_strings[8], S_strltrim(sample_strings[8]));
  ASSERT_STREQ(sample_strings[8], str_trim(sample_strings[8], NULL, 2));
  ASSERT_STREQ(sample_strings[8], S_strrtrim(sample_strings[8]));
  ASSERT_STREQ(sample_strings[8], str_trim(sample_strings[8], NULL, 3));
  ASSERT_STREQ(sample_strings[8], S_strtrim(sample_strings[8]));

  ASSERT_STREQ(sample_strings[9], str_trim(sample_strings[9], NULL, 1));
  ASSERT_STREQ(sample_strings[9], S_strltrim(sample_strings[9]));
  ASSERT_STREQ(sample_strings[9], str_trim(sample_strings[9], NULL, 2));
  ASSERT_STREQ(sample_strings[9], S_strrtrim(sample_strings[9]));
  ASSERT_STREQ(sample_strings[9], str_trim(sample_strings[9], NULL, 3));
  ASSERT_STREQ(sample_strings[9], S_strtrim(sample_strings[9]));

  ASSERT_STREQ("****trim", str_trim(sample_strings[10], "/", 2));
  ASSERT_STREQ("****trim", S_strrtrim_t(sample_strings[10], "/"));
  ASSERT_STREQ("trim////", str_trim(sample_strings[10], "*", 1));
  ASSERT_STREQ("trim////", S_strltrim_t(sample_strings[10], "*"));
  ASSERT_STREQ("trim", str_trim(sample_strings[10], "*/", 3));
  ASSERT_STREQ("trim", S_strtrim_t(sample_strings[10], "*/"));
  ASSERT_STREQ("ri", str_trim(sample_strings[10], "*/tm", 3));
  ASSERT_STREQ("ri", S_strtrim_t(sample_strings[10], "*/tm"));
}

TEST(STRING, join) {
  const char *str[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN",
                       "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

  ASSERT_STREQ(strjoin(str, 12, "*"),
               "JAN*FEB*MAR*APR*MAY*JUN*JUL*AUG*SEP*OCT*NOV*DEC");

  const char *delim[] = {"*", "- ", ","};
  ASSERT_STREQ(strjoin_a(str, 12, delim, 3),
               "JAN*FEB- MAR,APR*MAY- JUN,JUL*AUG- SEP,OCT*NOV- DEC");
  ASSERT_STREQ(strjoin_a(str, 12, delim, 2),
               "JAN*FEB- MAR*APR- MAY*JUN- JUL*AUG- SEP*OCT- NOV*DEC");
}

TEST(STRING, split) {
  const char str[] = "JAN,FEB,MAR,APR,MAY,JUN,JUL,AUG,SEP,OCT,NOV,DEC,";
  const char *expected[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN",
                            "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

  char **res = NULL;
  size_t count = 0;

  int ret = strsplit(str, ",", &res, &count);
  ASSERT_EQ(ret, 1);
  ASSERT_EQ(count, 12);
  ASSERT_NE(res, nullptr);
  for (size_t k = 0; k < count; k++)
    ASSERT_STREQ(res[k], expected[k]);

  for (size_t k = 0; k < count; k++)
    free(res[k]);

  free (res);
  res = NULL;

  ret = strsplit(str, NULL, &res, &count);
  ASSERT_EQ(ret, -1);
  ASSERT_EQ(count, 0);
  ASSERT_EQ(res, nullptr);
  if(res)
    free (res);
}

TEST(STRING, lower_upper) {
  std::string str("upper");
  ASSERT_STREQ("UPPER", S_strtoupper_r((char *)"upper"));

  char *cstr = new char[str.length() + 1];
  strcpy(cstr, str.c_str());
  S_strtoupper(cstr);
  ASSERT_STREQ("UPPER", cstr);
  ASSERT_STREQ("UPPER", S_strtoupper_r((char *)"uPPer"));
  ASSERT_STREQ("UPPER", S_strtoupper_r((char *)"uppeR"));

  delete[] cstr;
  ASSERT_STREQ("lower", S_strtolower_r((char *)"LOWER"));

  str = "LOWER";
  cstr = new char[str.length() + 1];
  strcpy(cstr, str.c_str());
  S_strtolower(cstr);
  ASSERT_STREQ("lower", cstr);

  ASSERT_STREQ("lower", S_strtolower_r((char *)"LoWer"));
  ASSERT_STREQ("lower", S_strtolower_r((char *)"loweR"));

  delete[] cstr;
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
