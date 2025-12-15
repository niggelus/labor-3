#include <stdio.h>
#include <stdlib.h>
#include "funktionen.h"   
#include "schreibliste.h" 
#include "datenbank.h"  
#include "lookup.h"  

int main() {
    int wahl = 0;

    // Initialisierungen
    dateiVorbereiten();
    printf("Info: 'messung.txt' wurde initialisiert.\n");
    
    // Lookup-Tabelle laden (Wichtig für die Interpolation!)
    if (ladeLookupTabelle("messung_1.csv") != 0) {
        printf("Warnung: 'messung_1.csv' konnte nicht geladen werden. Interpolation wird 0 ergeben.\n");
    }

    // Datenbank starten
    if (datenbank_initialisieren() != 0) {
        printf("Warnung: Datenbank konnte nicht gestartet werden.\n");
    }

    while (1) {
        printf("\n=========================================\n");
        printf("           HAUPTMENÜ                     \n");
        printf("=========================================\n");
        printf("1. Simulation starten (Aufgabe 1)\n");
        printf("2. Messung Sensor & Interpolation (Aufgabe 2)\n");
        printf("3. Datenbank als CSV exportieren\n");
        printf("4. Beenden\n");
        printf("-----------------------------------------\n");
        printf("Ihre Wahl: ");

        if (scanf("%d", &wahl) != 1) {
            while(getchar() != '\n'); 
            printf("Ungültige Eingabe! Bitte Zahl wählen.\n");
            continue;
        }
        while(getchar() != '\n'); // Puffer leeren

        switch (wahl) {
            case 1:
                starteSimulation(); 
                break;

            case 2:
                // HIER: Aufruf der neuen Funktion aus ReadComPort.c
                starteMessung(); 
                break;

            case 3:
                datenbank_export_csv("export_messungen.csv");
                break;

            case 4:
                datenbank_schliessen();
                printf("Programm wird beendet. Auf Wiedersehen!\n");
                return 0;

            default:
                printf("Unbekannte Option. Bitte 1, 2, 3 oder 4 wählen.\n");
                break;
        }
    }
    return 0;
}