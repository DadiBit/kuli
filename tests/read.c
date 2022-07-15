#define _KULI_DEBUG 1
#include "../src/kuli.h"

int main () {

    FILE *map = fopen("test.kulimap", "ar+");
    FILE *data = fopen("test.kulidata", "ar+");
    
    kuli_t k = {.map = map, .data = data};

    char buf[1024];
    kuli_entry_t entry = {
        .size = sizeof(buf),
        .data = buf,
    };

    while (1) {
        kuli.read(k, &entry, sizeof(buf)-1);
        if (feof(map)) break; // it's up to the user to test if th eof was reached
        entry.data[entry.size] = 0; // 0 terminate string
        printf("char[%d]: \"%s\"\n", entry.size, entry.data);
    }
    
    fclose(data);
    fclose(map);

    return 0;

}