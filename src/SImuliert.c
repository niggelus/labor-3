#include <stdio.h>
#include <stdlib.h> // für rand()
#include "schreibliste.h"
#include "datenbank.h"

#define MAX_MESSUNGEN 20

void starteSimulation(void) {
    
    

    printf("Starte Simulation (Aufgabe 1)...\n");
    printf("Dies ist der Simulations-Modus ohne echten Sensor.\n");

    for(int i = 1; i <= MAX_MESSUNGEN; i++)
    {
        float realerAbstand = 0.0f;
        
        printf("\n--- Messung %d von %d ---\n", i, MAX_MESSUNGEN);
        printf("Bitte gemessenen Abstand in Meter eingeben (z.B. 0.5): ");
        
        if (scanf("%f", &realerAbstand) != 1) {
            printf("Ungültige Eingabe! Bitte eine Zahl eingeben.\n");
            while(getchar() != '\n'); 
            i--; 
            continue;
        }
        while(getchar() != '\n'); 

        float sensorWert = ((float)rand() / RAND_MAX) * 2.0f; 
        float abweichung = realerAbstand - sensorWert;

        printf("Simuliert: Real=%.4f m | Sensor=%.4f m | Diff=%.4f m\n", 
               realerAbstand, sensorWert, abweichung);

       
        schreibeZeile(i, realerAbstand, sensorWert, abweichung);
        
        
        messung_speichern(i, realerAbstand, sensorWert, abweichung);
    }

    printf("\nSimulation abgeschlossen. Daten gespeichert.\n");
    
    }