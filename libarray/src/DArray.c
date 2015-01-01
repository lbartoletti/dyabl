#include <DArray.h>
#include <search.h>
#include <string.h>

DArray *DArray_new(size_t size) {
  DArray *array = malloc(sizeof(DArray));
  if (array) {
    array->data = NULL;
    array->size = size;
    array->nb_elem = 0;
    array->pos = 0;
    array->step = DEFAULT_STEP;
    array->compar = NULL;
  }

  return array;
}

DArray *DArray_sized_new(size_t size, size_t nb_elem) {
  DArray *ar = DArray_new(size);
  if (ar) {
    if (!DArray_grow(ar, size, false)) {
      free(ar);
      ar = NULL;
    } else {
      ar->nb_elem = nb_elem;
    }
  }
  return ar;
}

DArray *DArray_sized_step_new(size_t size, size_t nb_elem, size_t step) {
  DArray *ar = DArray_new(size);
  if (ar) {
    if (!DArray_grow(ar, size, false)) {
      free(ar);
      ar = NULL;
    } else {
      ar->nb_elem = nb_elem;
      DArray_set_step(ar, step);
    }
  }
  return ar;
}

DArray *DArray_new_init(size_t size, size_t nb_elem, size_t step,
                        int (*compar)(const void *, const void *)) {
  DArray *ar = DArray_new(size);
  if (ar) {
    if (!DArray_grow(ar, size, false)) {
      free(ar);
      ar = NULL;
    } else {
      ar->nb_elem = nb_elem;
      DArray_set_step(ar, step);
      ar->compar = compar;
    }
  }
  return ar;
}

void DArray_set_compar(DArray *array,
                       int (*compar)(const void *, const void *)) {
  if (array) {
    array->compar = compar;
  }
}

void DArray_set_step(DArray *array, size_t step) {
  if (array) {
    array->step = (step > 0 ? step : DEFAULT_STEP);
  }
}

int DArray_shrink(DArray *array, size_t size, bool use_step) {
  if (!array) {
    return DARRAY_ERR_NOARRAY;
  }
  if (size >= array->nb_elem) {
    return DARRAY_ERR;
  }

  size_t new_size;
  if (use_step && (size % array->step != 0)) {
    new_size = (size / array->step) * array->step;
  } else {
    new_size = size;
  }

  void *tmp = NULL;
  tmp = realloc(array->data, new_size * sizeof(void *));
  if (!tmp) {
    return DARRAY_ERR_MEMORY;
  }
  array->data = tmp;
  array->nb_elem = new_size;

  return DARRAY_SUCCESS;
}

int DArray_grow(DArray *array, size_t size, bool use_step) {
  if (!array) {
    return DARRAY_ERR_NOARRAY;
  }
  if (size <= array->nb_elem) {
    return DARRAY_ERR;
  }

  size_t new_size;
  size_t step_diff = (size - array->nb_elem);
  if (use_step && (step_diff % array->step != 0)) {
    new_size = array->nb_elem + step_diff * array->step;
  } else {
    new_size = size;
  }

  void *tmp = NULL;
  tmp = realloc(array->data, new_size * sizeof(void *));
  if (!tmp) {
    return DARRAY_ERR_MEMORY;
  }

  array->data = tmp;
  array->nb_elem = new_size;
  /*for (size_t i = array->pos; i < array->nb_elem; i++)
      array->data[i] = NULL;*/

  return DARRAY_SUCCESS;
}

bool DArray_shrink_to_fit(DArray *array) {
  if (!array) {
    return false;
  }
  if (array->pos == 0) {
    DArray_flush(array);
    return true;
  }
  void *tmp = NULL;
  tmp = realloc(array->data, array->pos * sizeof(void *));
  if (!tmp) {
    return false;
  }

  array->data = tmp;
  array->nb_elem = array->pos;
  return true;
}

void DArray_flush(DArray *array) {
  if (array) {
    if (array->data) {
      free(array->data);
    }
    array->pos = 0;
    array->nb_elem = 0;
    array->data = NULL;
  }
}

void DArray_free(DArray *array) {
  if (array) {
    DArray_flush(array);
    free(array);
  }
}

static bool check_if_need_to_resize(DArray *array, size_t nb) {
  if (!array) {
    return false;
  }

  if ((array->nb_elem == array->pos) ||
      (array->nb_elem - 1 < array->pos + nb)) {
    return true;
  }

  return false;
}

int DArray_append(DArray *array, const void *data) {
  if (!array) {
    return DARRAY_ERR_NOARRAY;
  }

  if (check_if_need_to_resize(array, 1)) {
    int ret = DArray_grow(array, array->nb_elem + 1, true);
    if (ret != DARRAY_SUCCESS) {
      return ret;
    }
  }
  array->data[array->pos] = DECONST(void *, data);
  array->pos++;

  return DARRAY_SUCCESS;
}

int DArray_prepend(DArray *array, const void *data) {
  if (!array) {
    return DARRAY_ERR_NOARRAY;
  }

  if (check_if_need_to_resize(array, 1)) {
    int ret = DArray_grow(array, array->nb_elem + 1, true);
    if (ret != DARRAY_SUCCESS) {
      return ret;
    }
  }

  if (array->pos == 0) {
    return DArray_append(array, data);
  }

  memmove(&array->data[1], array->data, array->pos * sizeof(void *));

  array->data[0] = DECONST(void *, data);
  array->pos++;

  return DARRAY_SUCCESS;
}

int DArray_insert_position(DArray *array, const void *data,
                           const size_t position) {
  if (!array) {
    return DARRAY_ERR_NOARRAY;
  }

  if (position == 0) {
    return DArray_prepend(array, data);
  }
  if (position == array->pos) {
    return DArray_append(array, data);
  }
  if (position > array->pos) {
    return DARRAY_ERR_NOTRANGE;
  }

  if (check_if_need_to_resize(array, 1)) {
    int ret = DArray_grow(array, array->nb_elem + 1, true);
    if (ret != DARRAY_SUCCESS) {
      return ret;
    }
  }

  memmove(&array->data[position + 1], &array->data[position],
          (array->pos - position) * sizeof(void *));

  array->data[position] = DECONST(void *, data);
  array->pos++;

  return DARRAY_SUCCESS;
}

int DArray_set_position(DArray *array, const void *data,
                        const size_t position) {
  if (!array) {
    return DARRAY_ERR_NOARRAY;
  }
  if (position > array->pos) {
    return DARRAY_ERR_NOTRANGE;
  }

  array->data[position] = DECONST(void *, data);
  return DARRAY_SUCCESS;
}

void *DArray_get_position(DArray *array, const size_t position) {
  if (!array || !array->data || (position > array->pos)) {
    return NULL;
  }

  return array->data[position];
}

int DArray_unique_insert(DArray *array, const void *data) {
  if (!array) {
    return DARRAY_ERR_NOARRAY;
  }
  if (array->compar == NULL) {
    return DARRAY_ERR_NOCOMPAR;
  }

  for (size_t i = 0; i < array->pos; i++) {
    if (array->compar(data, array->data[i]) == 0) {
      return DARRAY_ERR_EXIST;
    }
  }

  return DArray_append(array, data);
}

int DArray_remove_tail(DArray *array, void **data) {
  if (!array) {
    return DARRAY_ERR_NOARRAY;
  }

  if (data != NULL) {
    *data = array->data[array->pos - 1];
  }
  memset(&array->data[array->pos - 1], 0, sizeof(void *));
  array->pos--;
  return DARRAY_SUCCESS;
}

int DArray_remove_head(DArray *array, void **data) {
  if (!array) {
    return DARRAY_ERR_NOARRAY;
  }
  if (data != NULL) {
    *data = array->data[0];
  }

  memmove(&array->data[0], &array->data[1], array->pos-- * sizeof(void *));
  return DARRAY_SUCCESS;
}

int DArray_remove_position(DArray *array, void **data, const size_t position) {
  if (!array) {
    return DARRAY_ERR_NOARRAY;
  }
  if (position == 0) {
    return DArray_remove_head(array, data);
  }
  if (position == array->pos) {
    return DArray_remove_tail(array, data);
  }
  if (position > array->pos) {
    return DARRAY_ERR_NOTRANGE;
  }

  if (data != NULL) {
    *data = array->data[position];
  }

  memmove(&array->data[position], &array->data[position + 1],
          (array->pos - position - 1) * sizeof(void *));

  array->pos--;
  return DARRAY_SUCCESS;
}

int DArray_remove(DArray *array, void **data) {
  if (!array) {
    return DARRAY_ERR_NOARRAY;
  }
  if (array->compar == NULL) {
    return DARRAY_ERR_NOCOMPAR;
  }

  for (size_t i = 0; i < array->pos; i++) {
    if (array->compar(*data, array->data[i]) == 0) {
      return DArray_remove_position(array, data, i);
    }
  }

  return DARRAY_ERR;
}
