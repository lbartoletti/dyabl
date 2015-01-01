#include "SList.h"
#include <stdio.h>
#include <stdlib.h>
void affChar(const void *data) { printf("'%s' ", (char *)data); }

void testChar(void) {
  char text[][6] = {"Hello", " ", "World", "!"};

  /*
   * Liste de Char*
   */
  SList *listChar = SList_new();
  for (int i = 0; i < 4; i++) {
    printf("Ajout de : ");
    affChar((const void *)text[i]);
    puts("");
    SList_append(listChar, (const void *)text[i]);
  }
  puts("--------");
  /* Affichages */
  printf("\n--------\n");
  SList_display(listChar, affChar);
  printf("\n--------\n");
  SList_free(listChar);
}
