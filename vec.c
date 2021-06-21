#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vec.h"

struct Vec {
  void **data;     /* dynamic memory area containing the integers */
  size_t length;   /* how many integers are currently stored in data */
  size_t capacity; /* how many integers can be stored in data */
};

Vec *vec_new() {
  Vec *xs = malloc(sizeof(Vec));
  if (xs == NULL) {
    return NULL;
  }
  xs->capacity = 1;
  xs->length = 0;
  xs->data = malloc(xs->capacity * sizeof(void *));
  if (xs->data == NULL) {
    return NULL;
  }
  return xs;
}

void vec_free(Vec *xs) {
  while (vec_length(xs) > 0) {
    vec_pop(xs);
  }
  free(xs->data);
  free(xs);
}

void **vec_at(Vec *xs, size_t i) { return xs->data + i; }

size_t vec_length(Vec *xs) { return xs->length; }

size_t vec_capacity(Vec *xs) { return xs->capacity; }

bool vec_set_capacity(Vec *xs, size_t capacity) {
  if (xs->length <= capacity) {
    xs->capacity = capacity;
    xs->data = realloc(xs->data, capacity * sizeof(void *));
    return xs->data != NULL;
  }
  return true;
}

bool vec_push(Vec *xs, void *x) {
  if (xs->capacity == xs->length) {
    bool success = vec_set_capacity(xs, xs->capacity * 2);
    if (!success) {
      return false;
    }
  }
  xs->data[xs->length] = x;
  xs->length++;
  return true;
};

void vec_pop(Vec *xs) {
  xs->length--;
  void *x = *vec_at(xs, xs->length);
  free(x);
  if (xs->length * 2 == xs->capacity) {
    vec_set_capacity(xs, xs->length);
  }
}

void vec_remove(Vec *xs, size_t i) {
  void *x = *vec_at(xs, i);
  free(x);
  xs->length--;
  for (size_t j = i; j < xs->length; ++j) {
    *vec_at(xs, j) = *vec_at(xs, j + 1);
  }
}

void vec_print(Vec *xs) {
  printf("Vector at address %p has %ld elements and capacity %ld.\n", xs,
         vec_length(xs), vec_capacity(xs));
  for (size_t i = 0; i < vec_length(xs); ++i) {
    void **xi = vec_at(xs, i);
    printf("  vec[%ld] = %p (address %p)\n", i, *xi, xi);
  }
  printf("\n");
}
