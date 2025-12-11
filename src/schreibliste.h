#ifndef SCHREIBENDATEI_H
#define SCHREIBENDATEI_H

// Funktion, um die Datei beim Start einmal zu leeren/vorzubereiten
void dateiVorbereiten(void);

// Funktion, um eine Textzeile anzuhängen
void schreibeZeile(int nr, double real, double sensor, double abweichung);

#endif