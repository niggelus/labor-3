#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "schreibliste.h"
#include "datenbank.h" 

#define MAX_MESSUNGEN 20


void starteMessung(void) {
    
    

    const char* port = "/dev/ttyUSB0";
    int serial_port = open(port, O_RDWR);

    if (serial_port < 0) {
        printf("Fehler beim Öffnen von %s: %s\n", port, strerror(errno));
        printf("Tipp: Ist der Sensor eingesteckt?\n");
        return; 
    }

    struct termios tty;
    if (tcgetattr(serial_port, &tty) != 0) {
        printf("Fehler beim Abrufen der Einstellungen: %s\n", strerror(errno));
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

    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Fehler beim Setzen der Einstellungen: %s\n", strerror(errno));
        close(serial_port);
        return;
    }

    printf("\n--- Start der Messreihe (Sensor) ---\n");
    printf("Bitte Sensor positionieren.\n");

    for (int i = 1; i <= MAX_MESSUNGEN; i++) {
        float realerAbstand = 0.0f;
        float sensorWert = 0.0f;
        
        printf("\nMessung %d von %d\n", i, MAX_MESSUNGEN);
        printf("Bitte realen Abstand eingeben (in Meter): ");
        
        if (scanf("%f", &realerAbstand) != 1) {
            printf("Ungültige Eingabe! Bitte Zahl eingeben.\n");
            while(getchar() != '\n'); 
            i--; 
            continue;
        }
        while(getchar() != '\n'); 

        printf("Warte auf Daten vom Sensor... ");
        fflush(stdout); 
    

        tcflush(serial_port, TCIFLUSH);
        
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        
        
        int num_bytes = read(serial_port, buffer, sizeof(buffer) - 1);

        if (num_bytes < 0) {
            printf("\nLesefehler: %s\n", strerror(errno));
            i--; 
        } else {
            buffer[num_bytes] = '\0'; 
            sensorWert = strtof(buffer, NULL); 
            
            float abweichung = realerAbstand - sensorWert;

            printf("OK.\n");
            printf("  Real:       %.4f m\n", realerAbstand);
            printf("  Sensor:     %.4f m\n", sensorWert);
            printf("  Abweichung: %.4f m\n", abweichung);

            
            schreibeZeile(i, realerAbstand, sensorWert, abweichung);
            
           
            messung_speichern(i, realerAbstand, sensorWert, abweichung);
        }
    }

    printf("\nMessreihe fertig. Zurück zum Menü.\n");
    close(serial_port);
    
}