#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "datenbank.h"

static sqlite3 *db = NULL;

int datenbank_initialisieren(void) {
    int rc = sqlite3_open("messdaten.db", &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Kann Datenbank nicht öffnen: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    
    const char *sql = 
        "CREATE TABLE IF NOT EXISTS messungen ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "nr INTEGER,"
        "realer_abstand REAL,"
        "sensor_wert REAL,"
        "abweichung REAL"
        ");";

    char *err_msg = 0;
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL Fehler (Init): %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    
    printf("Datenbank erfolgreich initialisiert.\n");
    return 0;
}

void messung_speichern(int nr, float real, float sensor, float abweichung) {
    if (!db) return;

    char *err_msg = 0;
    char sql[256];
    
     snprintf(sql, sizeof(sql), 
             "INSERT INTO messungen (nr, realer_abstand, sensor_wert, abweichung) "
             "VALUES (%d, %.4f, %.4f, %.4f);", 
             nr, real, sensor, abweichung);

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL Fehler (Insert): %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf(" -> In DB gespeichert.\n");
    }
}

void datenbank_export_csv(const char* dateiname) {
    if (!db) return;

    FILE *fp = fopen(dateiname, "w");
    if (!fp) {
        perror("Fehler beim Öffnen der Export-Datei");
        return;
    }

    
    fprintf(fp, "Messung Nr.;Realer Abstand (m);Sensor Wert (m);Abweichung (m)\n");

    sqlite3_stmt *stmt;
    const char *sql = "SELECT nr, realer_abstand, sensor_wert, abweichung FROM messungen ORDER BY nr ASC";

    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "Fehler beim Abrufen der Daten: %s\n", sqlite3_errmsg(db));
        fclose(fp);
        return;
    }

   
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int nr = sqlite3_column_int(stmt, 0);
        double real = sqlite3_column_double(stmt, 1);
        double sensor = sqlite3_column_double(stmt, 2);
        double diff = sqlite3_column_double(stmt, 3);

        fprintf(fp, "%d;%.4f;%.4f;%.4f\n", nr, real, sensor, diff);
    }

    sqlite3_finalize(stmt);
    fclose(fp);
    printf("Daten erfolgreich nach '%s' exportiert.\n", dateiname);
}

void datenbank_schliessen(void) {
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }
}