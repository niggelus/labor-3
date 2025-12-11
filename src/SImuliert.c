#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h> // für rand()
#include "schreibliste.h"

// Maximale Anzahl der Messungen (Aufgabe 1)
#define MAX_MESSUNGEN 20

int main() 
{
    // 1. Datei vorbereiten (Header schreiben)
    dateiVorbereiten();
    
    printf("Starte Simulation (Aufgabe 1)...\n");
    printf("Dies ist der Simulations-Modus ohne echten Sensor.\n");

    // Schleife für 20 Messungen
    for(int i = 1; i <= MAX_MESSUNGEN; i++)
    {
        float realerAbstand = 0.0f;
        
        // 2. Benutzereingabe abfragen
        printf("\n--- Messung %d von %d ---\n", i, MAX_MESSUNGEN);
        printf("Bitte gemessenen Abstand in Meter eingeben (z.B. 0.5): ");
        
        // Einlesen
        if (scanf("%f", &realerAbstand) != 1) {
            printf("Ungültige Eingabe! Bitte eine Zahl eingeben.\n");
            while(getchar() != '\n'); // Puffer leeren
            i--; // Wiederholen
            continue;
        }
        while(getchar() != '\n'); // Enter entfernen

        // 3. Sensorwert simulieren (Zufallszahl zwischen 0 und 2 Meter)
        // (float)rand()/RAND_MAX gibt 0.0 bis 1.0 -> mal 2.0 -> 0.0 bis 2.0m
        float sensorWert = ((float)rand() / RAND_MAX) * 2.0f; 
        
        // Optional: Damit der Wert realistischer in der Nähe deiner Eingabe ist:
        // sensorWert = realerAbstand + (((float)rand()/RAND_MAX) * 0.1f - 0.05f); // +/- 5cm Fehler

        // 4. Abweichung berechnen
        float abweichung = realerAbstand - sensorWert;

        printf("Simuliert: Real=%.4f m | Sensor=%.4f m | Diff=%.4f m\n", 
               realerAbstand, sensorWert, abweichung);

        // 5. Speichern
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        
        // Format: MessungNr; Real; Sensor; Abweichung
        snprintf(buffer, sizeof(buffer), "%d;%.4f;%.4f;%.4f", 
                 i, realerAbstand, sensorWert, abweichung);
                 
        schreibeZeile(buffer);
    }

    printf("\nSimulation abgeschlossen. Daten gespeichert.\n");
    return 0;
}