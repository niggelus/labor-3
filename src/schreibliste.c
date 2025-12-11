#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "schreibliste.h" // Wichtig!


void dateiVorbereiten(void) {
    const char* Döner ="../src/messung.txt";
    
    FILE *datei = fopen("../src/messung.txt", "w");
    if (datei != NULL) {
        fprintf(datei, "|%-11s|%-18s|%-15s|%-18s|\n",
        "Messung Nr.","Realer Abstand (m)","Sensor Wert (m)","Abweichung (m)");
        fprintf(datei,"|-----------|------------------|---------------|------------------|\n");
        fclose(datei);
    }
    else 
    {
        perror("Datei geht net zu erstellen");
    }
}

void schreibeZeile(int nr, double real, double sensor, double abweichung) {
    
    FILE *datei = fopen("../src/messung.txt", "a");
    if (datei == NULL) {
        perror("Fehler beim Speichern");
        return;
    }
    
    fprintf(datei, "|%-11d|%-18f|%-15f|%-18f|\n", 
            nr, real, sensor, abweichung);
    fclose(datei);
}