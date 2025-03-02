#include <iostream>
#include <list>
#include <map>
#include "road.h"
#include "roadpoint.h"

class Database {
    public:
        Database() {}
        void insertRoad(const Road& road) {
            roads[road.getRoadId()] = road;
        }
        void insertRoadPoint(const RoadPoint& roadPoint) {
            roadPoints[roadPoint.getRefRoadId()][roadPoint.getSequence()] = roadPoint;
        }
        std::map<int, Road> getRoads() const {
            return roads;
        }
        std::map<int, std::map<int, RoadPoint>> getRoadPoints() const {
            return roadPoints;
        }
        void setTrackPoints(const std::list<GPSTrackPoint>& trackPoints) {
            this->trackPoints = trackPoints;
        }
        std::list<GPSTrackPoint>& getMutableTrackPoints() {
            return trackPoints;
        }
        std::list<GPSTrackPoint> getTrackPoints() const {
            return trackPoints;
        }
        unsigned getInvalidPoints() const {
            unsigned invalid = 0;
            for (auto& point: trackPoints) {
                if (point.getRefRoadId() == -1) {
                    invalid++;
                }
            }
            return invalid;
        }
        void print() {
            std::cout << "ROADS" << std::endl;
            for (auto& road: roads) {
                std::cout << road.second << std::endl;
            }
            std::cout << "ROAD_POINTS" << std::endl;
            for (auto& roadsequence: roadPoints) {
                for (auto& roadpoint: roadsequence.second) {
                    std::cout << roadpoint.second << std::endl;
                }
            }
            std::cout << "GPS_TRACK_POINTS" << std::endl;
            for (auto& point: trackPoints) {
                std::cout << point << std::endl;
            }
        }
    private:
        std::map<int, Road> roads;
        std::map<int, std::map<int, RoadPoint>> roadPoints;
        std::list<GPSTrackPoint> trackPoints;

};