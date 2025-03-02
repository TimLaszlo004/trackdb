# GPS Coordinates Linking to Roads

This C++ program is designed to process GPS coordinates from CSV files and link them to roads stored in an SQLite database. The program reads the GPS points from the provided CSV files, inserts them into the database, and attempts to match each GPS point to the closest road from the road network defined in the database.

## Features
- Parse and import GPS coordinates from CSV files.
- Connect each GPS point to the correct road found in the database

## How to compile & run
**compile:** `g++ -o main.exe main.cpp -l sqlite3`
**run:** `./main.exe`
