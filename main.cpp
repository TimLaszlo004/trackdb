#include <iostream> 
#include <sqlite3.h>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <list>
#include "gpstrackpoint.h"
#include "sessionhandler.h"

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

int executeSQL(sqlite3* db, const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return rc;
    }
    return SQLITE_OK;
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

int main()
{
    std::string input;
    // readCSV("track_1.csv");
    while (true) {
        std::cout << "Please enter CSV file name to read [or type exit]:" << std::endl;
        std::cin >> input;
        if (evaluate(input)) break;
    }
    return 0;
    sqlite3* DB;
    int returnCode = 0;
    returnCode = sqlite3_open("myroads.db", &DB);

    if (returnCode) {
        std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
        return (-1);
    }
    else
        std::cout << "Database (" << input << ") opened" << std::endl;
    sqlite3_close(DB);
    return (0); 
} 
