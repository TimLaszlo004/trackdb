#include <iostream> 
#include <sqlite3.h>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <list>
#include "gpstrackpoint.h"
#include "sessionhandler.h"
#include "database.h"

enum class ErrorType {
    InvalidFileNameException,
    InvalidCSVFormatException
};

std::list<GPSTrackPoint> readCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw ErrorType::InvalidFileNameException;
    }

    std::string line;
    bool firstLine = true;
    std::list<GPSTrackPoint> data;
    while (std::getline(file, line)) {
        std::vector<long double> row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ',')) {
            long double value;
            try {
                value = stold(cell);
            } catch (...) {
                throw ErrorType::InvalidCSVFormatException;
            }
            row.push_back(value);
        }
        if (firstLine) {
            firstLine = false;
            continue;
        }
        data.push_back(GPSTrackPoint(row[0], row[1], row[2], row[3]));
    }

    file.close();
    return data;
}

void insertToDB(){

}


bool evaluate(const std::string& input){
    if (SessionHandler::getInstance().isStringEndingSession(input)) return true;

    try {
        std::list<GPSTrackPoint> points = readCSV(input);
    } catch (ErrorType errorType) {
        // in this case we can handle the error more gracefully
        switch (errorType)
        {
        case ErrorType::InvalidFileNameException:
            std::cout << "File not found. In case you meant to exit the program, please use a valid session ending." << std::endl;
            SessionHandler::getInstance().printValidSessionEndings();
            break;
        case ErrorType::InvalidCSVFormatException:
            std::cout << "Unexpected file format. Please use a CSV file that is:\n"
                      << " - separated with ',' commas\n"
                      << " - the first line and only the first line is containing unused data\n"
                      << " - does only use double-parsable values and no strings after the first line\n"
                      << " - data is in the following order: timestamp,lat,lon,heading" << std::endl;
            break;
        default:
            break;
        }
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unknown error occured" << std::endl;
    }
    std::cout << std::endl;
    return false;
}

Database prepareDBValues (const std::string dbfile) {
    Database DB;
    // DB.createTable();
    return DB;
    sqlite3* sqldb;
    if (sqlite3_open("tracks.db", &sqldb) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(sqldb) << std::endl;
        exit(1);
    }
    // std::vector<Track> tracks;
    std::string queryPoints = "SELECT ROAD_PNT_ID, REF_ROAD_ID, SEQUENCE, LAT, LON, duration FROM ROAD_POINTS;";
    std::string queryRoads = "SELECT ROAD_ID, ROAD_NAME, ROAD_CATEGORY, IS_ONEWAY, SPEED_LIMIT_IN_KPH, ROAD_WIDTH_IN_METER FROM ROAD_POINTS;";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(sqldb, queryPoints.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(sqldb) << std::endl;
        exit(1);
    }

    // Execute the query and fetch results row by row
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Track track;
        track.id = sqlite3_column_int(stmt, 0);
        track.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        track.duration = sqlite3_column_int(stmt, 2);

        tracks.push_back(track); // Store the row in a vector
    }

    sqlite3_finalize(stmt); // Clean up statement
    sqlite3_close(db);
    return tracks;
}

int main()
{
    Database DB = prepareDBValues("myroad.db");
    std::string input;
    while (true) {
        std::cout << "Please enter CSV file name to read [or type exit]:" << std::endl;
        std::cin >> input;
        if (evaluate(input)) break;
    }
    return 0;
} 
