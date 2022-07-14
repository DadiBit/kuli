#ifndef _KULI__KULI_H
#define _KULI__KULI_H

#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <endian.h>
#include <stdlib.h>

// TODO: don't exit! Use `return`s of `kuli` static variable to save status
#define KULI_ASSERT(cond) if (cond) \
printf("`" #cond "` is true\n"), exit(-1);

typedef uint8_t byte_t;

typedef struct kuli_s {
    FILE* map;
    FILE* data;
} kuli_t;

typedef struct kuli_entry_s {
    uint32_t size;
    byte_t *data;
} kuli_entry_t;
#define kuli_entry(x) ((kuli_entry_t){.size = sizeof(x), .data = x})

typedef struct kuli_static_s {
    struct kuli_static_s (*first)(kuli_t k);
    struct kuli_static_s (*next)(kuli_t k);
    struct kuli_static_s (*prev)(kuli_t k);
    struct kuli_static_s (*this)(kuli_t k, kuli_entry_t *dst, uint32_t n);
    struct kuli_static_s (*append)(kuli_t k, kuli_entry_t *src);
} kuli_static_t;
extern kuli_static_t kuli;

#endif