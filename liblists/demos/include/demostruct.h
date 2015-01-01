
typedef struct _d {
  int c;
  char cc;
} test_struct;

void affStruct(const void *ma_struct) {
  test_struct *st = (test_struct *)ma_struct;
  printf("'%d %c' ", st->c, st->cc);
}

void testStruct(void) {
  test_struct st[] = {65, 'A', 66, 'B', 67, 'C', 68, 'D'};

  /*
   * Liste de Struct
   */
  SList *listStruct = SList_new();
  for (int i = 0; i < 4; i++) {
    printf("Ajout de : ");
    affStruct((const void *)&st[i]);
    puts("");
    SList_append(listStruct, (const void *)&(st[i]));
  }
  puts("--------");
  /* Affichages */
  printf("\n--------\n");
  SList_display(listStruct, affStruct);
  printf("\n--------\n");
  SList_free(listStruct);
}
