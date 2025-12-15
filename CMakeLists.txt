#cmake_minimum_required(VERSION 3.10)
#project("Hoehenmessung")

#add_executable(mini-test src/ReadComPort.c src/schreibliste.c)
#add_executable(ohne-test src/SImuliert.c src/schreibliste.c)
cmake_minimum_required(VERSION 3.10)
project(C_Projekt_Labor)

# --- NEU: Pfad für macOS (Homebrew) hinzufügen ---
# Prüfen, ob wir auf einem Mac sind (AppleClang oder Systemname)
if(APPLE)
    # Fügt typische Homebrew-Pfade hinzu, damit find_package fündig wird
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/sqlite") # Für M1/M2/M3 Macs
    list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/sqlite")    # Für Intel Macs
endif()

# SQLite Paket suchen
find_package(SQLite3 REQUIRED)

# Executable definieren
add_executable(LaborProgramm 
    src/main.c 
    src/SImuliert.c 
    src/ReadComPort.c 
    src/schreibliste.c
    src/datenbank.c
    src/lookup.c
)

# Bibliothek verlinken
target_link_libraries(LaborProgramm PRIVATE SQLite::SQLite3)