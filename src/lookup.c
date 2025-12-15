#include <stdio.h>
#include <stdlib.h>  // Nötig für abs()
#include "lookup.h"

// Globale Variablen definieren (Speicher reservieren)
LookupEintrag lookupTabelle[100];
int anzahlEintraege = 0;

// Funktion zum Laden der CSV-Daten
int ladeLookupTabelle(const char* dateiname) {
    FILE* datei = fopen(dateiname, "r");
    if (datei == NULL) {
        printf("Fehler: Konnte Datei %s nicht öffnen!\n", dateiname);
        return -1;
    }

    anzahlEintraege = 0;
    
    // Liest Zeile für Zeile: Sensorwert (Ganzzahl), Abstand (Fließkomma)
    while (fscanf(datei, "%d,%f", &lookupTabelle[anzahlEintraege].sensorWert, 
                                  &lookupTabelle[anzahlEintraege].abstand) == 2) {
        anzahlEintraege++;
        if (anzahlEintraege >= 100) break; 
    }

    fclose(datei);
    printf("Info: %d Einträge aus der Lookup-Tabelle geladen.\n", anzahlEintraege);
    return 0;
}


float berechneAbstand(int messwert) {
   
    if (anzahlEintraege == 0) {
        return 0.0f; 
    }

    int besterIndex = 0;
    
    int minDiff = abs(lookupTabelle[0].sensorWert - messwert);

    
    for (int i = 1; i < anzahlEintraege; i++) {
        int diff = abs(lookupTabelle[i].sensorWert - messwert);
        
        
        if (diff < minDiff) {
            minDiff = diff;
            besterIndex = i;
        }
    }

    
    return lookupTabelle[besterIndex].abstand;
}