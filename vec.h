#ifndef VEC_H
#define VEC_H

#include <stdbool.h>
#include <stddef.h>

/* Struct Declaration. The actual struct definition is hidden in `vec.c`.
 *
 * This prevents people, who are using our vector library, to directly access
 * the fields of this struct, like `capacity`. The only way to interact with the
 * vector is via the functions declared in this header.
 *
 * In other words: people using this library are not able to do stupid stuff
 * like changing the vector's capacity without also reallocating the memory area
 * of the vector.
 */
struct Vec;

/* Allows us to write `Vec` instead of `struct Vec`. */
typedef struct Vec Vec;

/* Returns a pointer to a freshly allocated vector of length 0 and capacity 1.
 * Returns NULL if we run out of memory.
 */
Vec *vec_new();

/* Free the dynamically allocated memory areas of `xs`. */
void vec_free(Vec *xs);

/* Returns a pointer to the element at index `i` of vector `xs`. */
void **vec_at(Vec *xs, size_t i);

/* Returns the number of elements currently stored in `xs`. */
size_t vec_length(Vec *xs);

/* Returns the vector's current capacity. */
size_t vec_capacity(Vec *xs);

/* Append a single element `x` at the end of the vector `xs`.
 *
 * If the vector's length is equal to its capacity, the vector's data is
 * reallocated at the double of its capacity to allow for amortized constant
 * runtime.
 *
 * Returns `false` if we run out memory, otherwise `true`.
 */
bool vec_push(Vec *xs, void *x);

/* Remove the last element of vector `xs` and return it.
 *
 * If the resulting vector's length is half of its capacity, the vector's
 * capacity is shrunk to its new length to save memory.
 */
void vec_pop(Vec *xs);

/* Like vec_pop, but removes the element at a specific index. */
void vec_remove(Vec *xs, size_t i);

/* Print the address, length, capacity, and elements of `xs`. */
void vec_print(Vec *xs);

/* Returns a pointer to the smallest element in `xs` which has an index greater
 * or equal to `begin` but less than `end`.
 *
 * If the range between `begin` and `end` is invalid or contains no elements,
 * then the NULL-pointer is returned.
 */
int *vec_min_between(Vec *xs, size_t begin, size_t end);

/* Sorts the elements of `xs` in increasing order. */
void vec_sort(Vec *xs);

#endif /* VEC_H */
