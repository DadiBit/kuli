#include "kuli.h"

/**
 * Seek to next entry size
 * @param k Kuli interface
 */
inline static void next_size(kuli_t k) {
    KULI_ASSERT(-1 == fseek(k.map, sizeof(uint32_t), SEEK_CUR));
}

/**
 * Seek to previous entry size
 * @param k Kuli interface
 */
inline static void prev_size(kuli_t k) {
    KULI_ASSERT(-1 == fseek(k.map, -sizeof(uint32_t), SEEK_CUR));
}

/**
 * Read the current entry size and seek to next entry size
 * @param k Kuli interface
 * @returns The entry size
 */
inline static uint32_t read_size(kuli_t k) {
    uint32_t le;
    KULI_ASSERT(-1 == fread(&le, sizeof(le), 1, k.map));
    // return the uint32 converted from little endian
    return le32toh(le);
}

/**
 * Append a new entry size and seek to end of map file
 * @param k Kuli interface
 * @param sz The new size
 */
inline static void append_size(kuli_t k, uint32_t sz) {
    // convert uint32 to little endian
    uint32_t le = htole32(sz);
    KULI_ASSERT(-1 == fwrite(&le, sizeof(le), 1, k.map));
}

/**
 * Seek to the next entry data/size
 * @param k Kuli interface
 * @returns Kuli interface
 */
inline static kuli_t next(kuli_t k) {
    uint32_t size = read_size(k);
    KULI_ASSERT(-1 == fseek(k.data, size, SEEK_CUR));
    return k;
}

/**
 * Seek to the next entry data/size
 * @param k Kuli interface
 * @returns Kuli interface
 */
inline static kuli_t prev(kuli_t k) {
    prev_size(k);
    uint32_t size = read_size(k);
    prev_size(k);
    // if we were to use `-size` we would mess up, as it is a uin32_t and not a int32_t...
    int64_t _size = size & INT32_MAX;
    KULI_ASSERT(-1 == fseek(k.data, -_size, SEEK_CUR));
    return k;
}

/**
 * Read the current entry data and seek to the the next entry data/size
 * @param k Kuli interface
 * @param dst The entry destination pointer
 * @param n The maximum number of data bytes that should be read
 * @returns Kuli interface
 */
inline static kuli_t read(kuli_t k, kuli_entry_t *dst, uint32_t n) {
    KULI_ASSERT(dst == NULL);
    // printf("Entry id: %ld\n", ftell(k.map)/sizeof(uint32_t)); // DEBUG
    dst->size = read_size(k);
    //sanify size
    if (dst->size > n)
        dst->size = n; // ! FIX: if n is used than fread moves only by n bytes, messing current position in data file
    KULI_ASSERT(-1 == fread(dst->data, dst->size, 1, k.data));
    return k;
}

/**
 * Append a new entry and seek and seek to end of map/data file
 * @param k Kuli interface
 * @param src The entry source pointer
 * @returns Kuli interface
 */
inline static kuli_t append(kuli_t k, kuli_entry_t *src) {
    KULI_ASSERT(src == NULL);
    append_size(k, src->size);
    KULI_ASSERT(-1 == fwrite(src->data, src->size, 1, k.data));
    // TODO: care about number of written/read bytes
    return k;
}

/**
 * Rewind to first entry (start of file)
 * @param k Kuli interface
 * @returns Kuli interface
 */
inline static kuli_t first(kuli_t k) {
    rewind(k.map);
    rewind(k.data);
    return k;
}

/**
 * Create new kuli interface (kuli_t)
 * @param map The map file
 * @param data The data file
 * @returns Kuli interface
 */
inline static kuli_t new(FILE *map, FILE *data) {
    kuli_t k = {
        .map    = map,
        .data   = data,
    };
    return k;
}

kuli_static_t kuli = {
    .new    = new,
    .first  = first,
    .next   = next,
    .prev   = prev,
    .read   = read,
    .append = append,
};