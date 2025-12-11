#include <stdio.h>
#include <stdlib.h>
#include "funktionen.h"   
#include "schreibliste.h" 
#include "datenbank.h"    

int main() {
    int wahl = 0;

    
    dateiVorbereiten();
    printf("Info: 'messung.txt' wurde initialisiert.\n");

    
    if (datenbank_initialisieren() != 0) {
        printf("Warnung: Datenbank konnte nicht gestartet werden.\n");
        
    }

    
    while (1) {
        printf("\n=========================================\n");
        printf("           HAUPTMENÜ                     \n");
        printf("=========================================\n");
        printf("1. Simulation starten du huan\n");
        printf("2. Messung Sensor \n");
        printf("3. Daten csv\n");
        printf("4. ende\n");
        printf("-----------------------------------------\n");
        printf("Ihre Wahl: ");

        if (scanf("%d", &wahl) != 1) {
            while(getchar() != '\n'); 
            printf("Ungültige Eingabe! Bitte Zahl wählen.\n");
            continue;
        }
        while(getchar() != '\n'); 

        switch (wahl) {
            case 1:
                starteSimulation(); 
                break;

            case 2:
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