#include "CHashTab.h"

CHashTab *CHashTabinit(int buckets, int (*hash)(const void *),
                       int (*compar)(const void *, const void *),
                       void (*destroy)(void *)) {
  CHashTab *chtab;
  int i;

  if ((chtab = malloc(sizeof(struct chashtab))) == NULL) {
    return NULL;
  }

  if ((chtab->table = malloc((size_t)buckets * sizeof(SList *))) == NULL) {
    free(chtab);
    return NULL;
  }

  chtab->buckets = buckets;

  for (i = 0; i < chtab->buckets; i++) {
    if ((chtab->table[i] = SList_new_init(destroy, compar)) == NULL) {
      for (int j = 0; j < i; j++) {
        SList_free(chtab->table[j]);
      }
      free(chtab->table);
      free(chtab);
      return NULL;
    }
  }

  chtab->hash = hash;
  chtab->compar = compar;
  chtab->destroy = destroy;
  chtab->size = 0;

  return chtab;
}

void CHashTabdestroy(CHashTab *chtab) {
  int i;

  for (i = 0; i < chtab->buckets; ++i) {
    SList_free(chtab->table[i]);
  }

  free(chtab->table);
  free(chtab);
}

int CHashTabinsert(CHashTab *chtab, const void *data) {
  int bucket;
  int retval;
  void *tmp;

  tmp = DECONST(void *, data);

  if (CHashTablookup(chtab, &tmp) == 0) {
    return 1;
  }

  /* Hash the key */
  bucket = chtab->hash(data) % chtab->buckets;

  if ((retval = SList_append(chtab->table[bucket], data)) == SLIST_SUCCESS) {
    chtab->size++;
  }

  return retval;
}

int CHashTabremove(CHashTab *chtab, void **data) {
  int bucket;
  int retval;

  /* Hash the key */
  bucket = chtab->hash(*data) % chtab->buckets;

  /* Remove the node */
  if ((retval = SList_remove(chtab->table[bucket], data)) ==
      SLIST_SUCCESS) { /* Node removal successful.. */
    chtab->size--;
  }

  return retval;
}

int CHashTablookup(const CHashTab *chtab, void **data) {
  int bucket;
  SList_elem *tmpnode;

  /* Hash the key */
  bucket = chtab->hash(*data) % chtab->buckets;

  if ((tmpnode = SList_find(chtab->table[bucket], *data)) !=
      NULL) /* Data found */
  {
    *data = (tmpnode->data); /* Pass data back to caller */
    return 0;
  }

  return -1;
}

void CHashTabprint(CHashTab *chtab, void (*callback)(const void *)) {
  (void)chtab;
  (void)callback;
  /*int i;

  for (i = 0; i < chtab->buckets; ++i) {
      printf ("\nBucket #%03d: ", i);
      SListtraverse (chtab->table[i], callback, SList_FWD);
  }*/
}

int CHashTabsize(CHashTab *chtab) { return chtab->size; }
