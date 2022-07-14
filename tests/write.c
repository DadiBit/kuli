#include "../src/kuli.h"

int main () {

    FILE *map = fopen("test.kulimap", "ar+");
    FILE *data = fopen("test.kulidata", "ar+");
    
    kuli_t k = {.map = map, .data = data};

    kuli_entry_t ent1 = kuli_entry("Hello, World!");
    kuli_entry_t ent2 = kuli_entry("Kuli is fast to append data!");
    kuli_entry_t ent3 = kuli_entry("This is the last entry :(");

    /* Please note that we don't want to store the 0 termination */
    ent1.size--;
    ent2.size--;
    ent3.size--;

    kuli
    .append(k, &ent1)
    .append(k, &ent2)
    .append(k, &ent3);

    fclose(data);
    fclose(map);

    return 0;

}