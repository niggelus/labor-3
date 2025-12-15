float berechneAbstand(int messwert) {
    if (anzahlEintraege < 2) {
        return 0.0f; // Nicht genug Daten zum Interpolieren
    }

    // 1. Suche die passenden Stützstellen (Nachbarn)
    int i;
    for (i = 0; i < anzahlEintraege - 1; i++) {
        // Wir suchen den Bereich, wo: Wert[i] <= messwert <= Wert[i+1]
        // Hinweis: Die CSV sollte nach Sensorwerten sortiert sein!
        if (lookupTabelle[i].sensorWert <= messwert && lookupTabelle[i+1].sensorWert >= messwert) {
            
            // 2. Lineare Interpolation
            int x0 = lookupTabelle[i].sensorWert;
            int x1 = lookupTabelle[i+1].sensorWert;
            float y0 = lookupTabelle[i].abstand;
            float y1 = lookupTabelle[i+1].abstand;

            // Formel: y = y0 + (x - x0) * (y1 - y0) / (x1 - x0)
            float ergebnis = y0 + (float)(messwert - x0) * (y1 - y0) / (float)(x1 - x0);
            return ergebnis;
        }
    }

    // Fallback: Wenn Wert außerhalb der Tabelle liegt (z.B. kleiner als min oder größer als max)
    // Hier geben wir einfach den nächstgelegenen Randwert zurück oder 0.
    if (messwert < lookupTabelle[0].sensorWert) return lookupTabelle[0].abstand;
    return lookupTabelle[anzahlEintraege - 1].abstand;
}