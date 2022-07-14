#define _KULI_DEBUG 1
#include "../src/kuli.h"

int main () {

    FILE *map = fopen("test.kulimap", "ar+");
    FILE *data = fopen("test.kulidata", "ar+");
    
    kuli_t k = {.map = map, .data = data};

    byte_t buf[1024];
    kuli_entry_t entry = {
        .size = sizeof(buf),
        .data = buf,
    };

    while (!feof(map)) {
        kuli.this(k, &entry, sizeof(buf)-1);
        entry.data[entry.size] = 0; // 0 terminate string
        printf("char[%d]: \"%s\"\n", entry.size, entry.data);
    }
    // last entry should be printed twice

    fclose(data);
    fclose(map);

    return 0;

}