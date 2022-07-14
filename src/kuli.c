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
inline static uint32_t this_size(kuli_t k) {
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
 */
inline static kuli_static_t next(kuli_t k) {
    uint32_t size = this_size(k);
    KULI_ASSERT(-1 == fseek(k.data, size, SEEK_CUR));
    return kuli;
}

/**
 * Seek to the next entry data/size
 * @param k Kuli interface
 */
inline static kuli_static_t prev(kuli_t k) {
    prev_size(k);
    uint32_t size = this_size(k);
    prev_size(k);
    int64_t _size = size & INT32_MAX;
    KULI_ASSERT(-1 == fseek(k.data, -_size, SEEK_CUR));
    return kuli;
}

/**
 * Read the current entry data and seek to the the next entry data/size
 * @param k Kuli interface
 * @param dst The entry destination pointer
 * @param n The maximum number of data bytes that should be read
 */
inline static kuli_static_t this(kuli_t k, kuli_entry_t *dst, uint32_t n) {
    KULI_ASSERT(dst == NULL);
    dst->size = this_size(k);
    //sanify size
    if (dst->size > n)
        dst->size = n;
    KULI_ASSERT(-1 == fread(dst->data, dst->size, 1, k.data));
    return kuli;
}

/**
 * Append a new entry and seek and seek to end of map/data file
 * @param k Kuli interface
 * @param src The entry source pointer
 * @param sz The new size
 */
inline static kuli_static_t append(kuli_t k, kuli_entry_t *src) {
    KULI_ASSERT(src == NULL);
    append_size(k, src->size);
    KULI_ASSERT(-1 == fwrite(src->data, src->size, 1, k.data));
    // TODO: care about number of written/read bytes
    return kuli;
}

/**
 * Rewind to first entry (start of file)
 * @param k Kuli interface
 */
inline static kuli_static_t first(kuli_t k) {
    rewind(k.map);
    rewind(k.data);
    return kuli;
}

kuli_static_t kuli = {
    .first = first,
    .next = next,
    .prev = prev,
    .this = this,
    .append = append,
};