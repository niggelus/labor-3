#ifndef LOOKUP_H
#define LOOKUP_H


typedef struct {
    int sensorWert;   
     float abstand;    
} LookupEintrag;

extern LookupEintrag lookupTabelle[100]; 
extern int anzahlEintraege;


int ladeLookupTabelle(const char* dateiname);
float berechneAbstand(int messwert);

#endif