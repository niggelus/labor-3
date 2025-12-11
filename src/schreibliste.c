#include <stdio.h>
#include <stdlib.h>
#include "schreibliste.h" // Wichtig!


void dateiVorbereiten(void) {
    FILE *datei = fopen("../src/messung.txt", "w");
    if (datei != NULL) {
        fprintf(datei, "Messung Nr.;Realer Abstand (m);Sensor Wert (m);Abweichung (m)\n");
        fclose(datei);
    }
}

void schreibeZeile(const char* text) {
    
    FILE *datei = fopen("../src/messung.txt", "a");
    if (datei == NULL) {
        perror("Fehler beim Speichern");
        return;
    }
    
    fprintf(datei, "%s\n", text);
    fclose(datei);
}