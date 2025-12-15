#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "schreibliste.h"
#include "datenbank.h"
#include "lookup.h"  // Wichtig für berechneAbstand()

void starteMessung(void) {
    // --- 1. Verbindung zum Sensor ---
    const char* port = "/dev/ttyUSB0";
    int serial_port = open(port, O_RDWR);

    // Fehlerbehandlung, falls Sensor nicht dran ist (damit du trotzdem testen kannst)
    if (serial_port < 0) {
        printf("Warnung: Konnte %s nicht öffnen (%s).\n", port, strerror(errno));
        printf("-> Der Modus läuft im SIMULATIONS-Betrieb (Zufallswerte).\n");
    } else {
        // Serielle Konfiguration
        struct termios tty;
        if (tcgetattr(serial_port, &tty) != 0) {
            printf("Fehler bei tcgetattr: %s\n", strerror(errno));
            close(serial_port);
            return;
        }
        cfsetispeed(&tty, B115200);
        cfsetospeed(&tty, B115200);
        tty.c_cflag &= ~PARENB;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CSIZE;
        tty.c_cflag |= CS8;
        tty.c_cflag |= CREAD | CLOCAL;
        tty.c_lflag |= ICANON;
        tty.c_lflag &= ~(ECHO | ECHOE | ISIG);
        tcsetattr(serial_port, TCSANOW, &tty);
    }

    // --- 2. CSV-Datei für Excel vorbereiten ---
    FILE *csvDatei = fopen("messung_2.csv", "w");
    if (csvDatei == NULL) {
        printf("Fehler: Konnte 'messung_2.csv' nicht erstellen!\n");
        if (serial_port >= 0) close(serial_port);
        return;
    }
    // Header schreiben: Semikolon getrennt für deutsches Excel
    fprintf(csvDatei, "Messung Nr.;Soll-Abstand (m);Sensor-Rohwert;Ist-Abstand Interpoliert (m)\n");

    printf("\n==============================================\n");
    printf("   MANUELLE MESSREIHE (Interpolation + Excel)\n");
    printf("==============================================\n");
    printf("Steuerung:\n");
    printf(" - Gib den 'echten' Abstand ein (Soll-Wert)\n");
    printf(" - Drücke [ENTER], um den Sensor auszulesen\n");
    printf(" - Gib 'q' ein, um zu beenden\n");

    int messungNr = 1;
    char eingabe[50];

    // --- 3. Die Mess-Schleife ---
    while (1) {
        printf("\n----------------------------------------------\n");
        printf("MESSUNG %d\n", messungNr);
        
        // A) Soll-Wert abfragen
        printf(">> Bitte realen Abstand (Soll) in Meter eingeben: ");
        if (fgets(eingabe, sizeof(eingabe), stdin) == NULL) break;

        // Prüfen auf 'q' für Quit
        if (eingabe[0] == 'q' || eingabe[0] == 'Q') {
            printf("Beende Messreihe...\n");
            break;
        }

        float realerAbstand = strtof(eingabe, NULL);

        // B) Auf Mess-Bestätigung warten
        printf("   Drücken Sie [ENTER] für die Messung...");
        fgets(eingabe, sizeof(eingabe), stdin);

        // C) Sensor lesen (oder simulieren)
        if (serial_port >= 0) tcflush(serial_port, TCIFLUSH);
        
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        int num_bytes = -1;

        if (serial_port >= 0) {
            num_bytes = read(serial_port, buffer, sizeof(buffer) - 1);
        } else {
            // Simulation: Erzeugt einen Rohwert passend zum Abstand + Rauschen
            // (Damit du Werte bekommst, auch wenn der Sensor fehlt)
            // Annahme: Sensorwert ~ Abstand * 1000 (rein fiktiv für Test)
            int fakeRoh = (int)(realerAbstand * 500) + (rand() % 20);
            snprintf(buffer, 20, "%d", fakeRoh);
            num_bytes = strlen(buffer);
        }

        if (num_bytes > 0) {
            buffer[num_bytes] = '\0';
            
            // Wert parsen
            float sensorRohFloat = strtof(buffer, NULL);
            int rohWert = (int)sensorRohFloat;

            // D) Interpolieren (Lookup-Tabelle nutzen)
            float interpoliert = berechneAbstand(rohWert);

            // Ausgabe
            printf("   -> Sensor Rohwert: %d\n", rohWert);
            printf("   -> Interpoliert:   %.4f m\n", interpoliert);
            printf("   -> Abweichung:     %.4f m\n", realerAbstand - interpoliert);

            // E) Speichern in CSV (für Excel Plot)
            fprintf(csvDatei, "%d;%.4f;%d;%.4f\n", 
                    messungNr, realerAbstand, rohWert, interpoliert);

            // F) Speichern in Datenbank (optional, aber sauber)
            messung_speichern(messungNr, realerAbstand, (float)rohWert, realerAbstand - interpoliert);

            messungNr++;
        } else {
            printf("Fehler: Keine Daten vom Sensor empfangen.\n");
        }
    }

    // Aufräumen
    fclose(csvDatei);
    if (serial_port >= 0) close(serial_port);
    printf("\nDaten wurden in 'messung_2.csv' gespeichert.\n");
}