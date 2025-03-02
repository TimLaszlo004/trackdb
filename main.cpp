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
#include "geomcalculations.h"

constexpr const char* DB_NAME = "road.db";
unsigned gps_track_id = 0;

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
        
        if (firstLine) {
            firstLine = false;
            continue;
        }
        while (std::getline(ss, cell, ',')) {
            long double value;
            try {
                value = stold(cell); // TODO: not everything is double
            } catch (...) {
                throw ErrorType::InvalidCSVFormatException;
            }
            row.push_back(value);
        }
        data.push_back(GPSTrackPoint(row[0], row[1], row[2], row[3]));
    }

    file.close();
    return data;
}

void insertIntoDB(const GPSTrackPoint& trackpoint) {
    sqlite3* sqldb;
    if (sqlite3_open(DB_NAME, &sqldb) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(sqldb) << std::endl;
        exit(1);
    }
    const char* sql = R"(
        INSERT INTO GPS_TRACK_POINTS (GPS_TRK_ID, REF_ROAD_ID, GPS_TIMESTAMP, DIRECTION_FROM_NORHT_IN_DEGREE, LAT, LON, MATCHING_ERR_CODE)
        VALUES (?, ?, ?, ?, ?, ?, ?);
    )";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(sqldb, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(sqldb) << std::endl;
        exit(1);
    }

    sqlite3_bind_int(stmt, 1, gps_track_id);
    if (trackpoint.getRefRoadId() >= 0) {
        sqlite3_bind_int(stmt, 2, trackpoint.getRefRoadId());
    } else {
        sqlite3_bind_null(stmt, 2);
    }
    sqlite3_bind_int(stmt, 3, trackpoint.getTimestamp());
    sqlite3_bind_double(stmt, 4, static_cast<double>(trackpoint.getHeading()));
    sqlite3_bind_double(stmt, 5, static_cast<double>(trackpoint.getLat()));
    sqlite3_bind_double(stmt, 6, static_cast<double>(trackpoint.getLon()));


    sqlite3_bind_int(stmt, 7, static_cast<int>(trackpoint.getError()));

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    if (!success) {
        std::cerr << "Error inserting data: " << sqlite3_errmsg(sqldb) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(sqldb);
    gps_track_id++;
}

void calculateRefRoadIDsAndInsert(Database& DB) {
    for (auto& trackpoint: DB.getMutableTrackPoints()) {
        bool isRoadFound = false;
        for (auto& roadsequences: DB.getRoadPoints()) {
            RoadPoint previousPoint;
            bool isFirst = true;
            for (auto& roadpoint: roadsequences.second) {
                if (isFirst) {
                    previousPoint = roadpoint.second;
                    isFirst = false;
                    continue;
                }
                if (GeometryCalculations::isPointInsideRoad(previousPoint.getLat(), previousPoint.getLon(),
                                                            roadpoint.second.getLat(), roadpoint.second.getLon(),
                                                            trackpoint.getLat(), trackpoint.getLon(),
                                                            DB.getRoads()[roadpoint.second.getRefRoadId()].getRoadWidthInMeter())) {
                    trackpoint.setRefRoadId(roadpoint.second.getRefRoadId());
                    isRoadFound = true;
                    break;
                }
                previousPoint = roadpoint.second;
            }
            if (isRoadFound) {
                break;
            }
        }
        if (!isRoadFound) {
            trackpoint.setError(NOT_FOUND);
        }
        insertIntoDB(trackpoint);
    }
}

bool evaluateAndCalculate(const std::string& input, Database& DB){
    if (SessionHandler::getInstance().isStringEndingSession(input)) return true;

    try {
        std::list<GPSTrackPoint> points = readCSV(input);
        DB.setTrackPoints(points);
        calculateRefRoadIDsAndInsert(DB);
        std::cout << "STATISTICS:\n"
              << "Total processed GPS points: " << DB.getTrackPoints().size()
              << "\nInvalid points: " << DB.getInvalidPoints()
              << std::endl;
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

Database prepareDBValues () {
    Database DB;
    sqlite3* sqldb;
    if (sqlite3_open(DB_NAME, &sqldb) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(sqldb) << std::endl;
        exit(1);
    }
    // load data from roads
    {
        const char* queryRoads = "SELECT ROAD_ID, ROAD_NAME, ROAD_CATEGORY, IS_ONEWAY, SPEED_LIMIT_IN_KPH, ROAD_WIDTH_IN_METER FROM ROADS;";
        
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(sqldb, queryRoads, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Error preparing statement: " << sqlite3_errmsg(sqldb) << std::endl;
            exit(1);
        }
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Road road(sqlite3_column_int(stmt, 0),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                sqlite3_column_int(stmt, 2),
                sqlite3_column_int(stmt, 3),
                sqlite3_column_int(stmt, 4),
                sqlite3_column_int(stmt, 5)
            );
            
            DB.insertRoad(road);
        }
        
        sqlite3_finalize(stmt);
    }
    // load data from road_points
    {
        const char* queryPoints = "SELECT ROAD_PNT_ID, REF_ROAD_ID, SEQUENCE, LAT, LON FROM ROAD_POINTS;";
        
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(sqldb, queryPoints, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Error preparing statement: " << sqlite3_errmsg(sqldb) << std::endl;
            exit(1);
        }
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            RoadPoint roadPoint(sqlite3_column_int(stmt, 0),
                sqlite3_column_int(stmt, 1),
                sqlite3_column_int(stmt, 2),
                std::stold(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))),
                std::stold(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)))
            );
            
            DB.insertRoadPoint(roadPoint);
        }
        
        sqlite3_finalize(stmt);
    }

    sqlite3_close(sqldb);
    return DB;
}

void deletePrevious() {
    Database DB;
    sqlite3* sqldb;
    if (sqlite3_open(DB_NAME, &sqldb) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(sqldb) << std::endl;
        exit(1);
    }
    const char* queryDel = "DELETE FROM GPS_TRACK_POINTS";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(sqldb, queryDel, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(sqldb) << std::endl;
        exit(1);
    }

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    if (!success) {
        std::cerr << "Error deleting data: " << sqlite3_errmsg(sqldb) << std::endl;
    }

    sqlite3_finalize(stmt);

    sqlite3_close(sqldb);
}

int main()
{
    deletePrevious();
    Database DB = prepareDBValues();
    // DB.print();
    std::string input;
    while (true) {
        std::cout << "Please enter CSV file name to read [or type exit]:" << std::endl;
        std::cin >> input;
        if (evaluateAndCalculate(input, DB)) break;
    }
    // DB.print();
    
    return 0;
} 
